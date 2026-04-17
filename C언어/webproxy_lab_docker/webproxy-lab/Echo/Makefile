CC = gcc
CFLAGS = -g -Wall -I ..
LDFLAGS = -lpthread

all: server client

server: echoserver_todo.c ../csapp.c ../csapp.h
	$(CC) $(CFLAGS) echoserver_todo.c ../csapp.c -o echoserver $(LDFLAGS)

client: echoclient_todo.c ../csapp.c ../csapp.h
	$(CC) $(CFLAGS) echoclient_todo.c ../csapp.c -o echoclient $(LDFLAGS)

clean:
	rm -f echoserver echoclient *.o *~

