/* anon.c: Implementation of page for non-disk image (a.k.a. anonymous page). */

#include "vm/vm.h"
#include "devices/disk.h"
#include "threads/palloc.h"
#include "threads/malloc.h"
#include "userprog/process.h"
#include "lib/kernel/bitmap.h"
#include "threads/vaddr.h"
#include "debug.h"


#define SECTORS_PER_PAGE (PGSIZE / DISK_SECTOR_SIZE)
/* DO NOT MODIFY BELOW LINE */
static struct disk *swap_disk;
static struct bitmap *swap_table;
static bool anon_swap_in (struct page *page, void *kva);
static bool anon_swap_out (struct page *page);
static void anon_destroy (struct page *page);

/* DO NOT MODIFY this struct */
static const struct page_operations anon_ops = {
	.swap_in = anon_swap_in,
	.swap_out = anon_swap_out,
	.destroy = anon_destroy,
	.type = VM_ANON,
};

/* Initialize the data for anonymous pages */
void
vm_anon_init (void) {
	/* TODO: Set up the swap_disk. */
	swap_disk = disk_get (1, 1);
	if (swap_disk == NULL)PANIC ("No swap disk");
    size_t swap_slot_count = disk_size (swap_disk) / SECTORS_PER_PAGE;

    swap_table = bitmap_create (swap_slot_count);

    if (swap_table == NULL)
        PANIC ("No swap bitmap");
}

/* Initialize the file mapping */
bool
anon_initializer (struct page *page, enum vm_type type, void *kva) {
	/* Set up the handler */
	struct anon_page *anon_page = &page->anon;
	page->operations = &anon_ops;
	anon_page->swap_slot = -1;
	return true;
}

/* Swap in the page by read contents from the swap disk. */
static bool
anon_swap_in (struct page *page, void *kva) {
	struct anon_page *anon_page = &page->anon;
	if (anon_page->swap_slot == (size_t) -1)return true;

	size_t slot = anon_page->swap_slot;
	disk_sector_t sector = slot * SECTORS_PER_PAGE;
	uint8_t *kaddr = kva;
	
	for (size_t i = 0; i < SECTORS_PER_PAGE; i++){
		disk_read (swap_disk, sector + i, kaddr + i * DISK_SECTOR_SIZE);
	}
	bitmap_reset (swap_table, slot);
	anon_page->swap_slot = -1;
	return true;
}

/* Swap out the page by writing contents to the swap disk. */
static bool
anon_swap_out (struct page *page) {
	struct anon_page *anon_page = &page->anon;
	size_t slot = bitmap_scan_and_flip (swap_table, 0, 1, false);
	if (slot == BITMAP_ERROR)return false;

	disk_sector_t sector = slot * SECTORS_PER_PAGE;
	uint8_t *kva = page->frame->kva;

	for (size_t i = 0; i < SECTORS_PER_PAGE; i++){
		disk_write (swap_disk, sector + i, kva + i * DISK_SECTOR_SIZE);
	}
	anon_page->swap_slot = slot;
	return true;
}

/* Destroy the anonymous page. PAGE will be freed by the caller. */
static void
anon_destroy (struct page *page) {
	if (page->frame != NULL) {
		free (page->frame);
		page->frame = NULL;
	}
}
