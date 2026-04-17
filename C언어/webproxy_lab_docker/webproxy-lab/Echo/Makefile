CC = gcc
CFLAGS = -g -Wall -I ..
LDFLAGS = -lpthread

# make server : Echo 서버만 빌드
# make client : 선택 연습용 Echo 클라이언트만 빌드
# make clean  : 빌드 결과 삭제

all: server client

server: echoserver_todo.c ../csapp.c ../csapp.h
	$(CC) $(CFLAGS) echoserver_todo.c ../csapp.c -o echoserver $(LDFLAGS)

client: echoclient_todo.c ../csapp.c ../csapp.h
	$(CC) $(CFLAGS) echoclient_todo.c ../csapp.c -o echoclient $(LDFLAGS)

clean:
	rm -f echoserver echoclient *.o *~
