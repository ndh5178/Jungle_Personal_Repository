# Echo 서버 TODO 연습

이 폴더는 CS:APP 11장 네트워크 프로그래밍을 연습하기 위한 작은 예제 폴더야.

목표:

1. 반복형 Echo 서버를 직접 구현한다.
2. `Open_listenfd`, `Accept`, `Getnameinfo`, `Rio_readinitb`,
   `Rio_readlineb`, `Rio_writen`의 흐름을 몸으로 익힌다.
3. 나중에 Tiny 서버로 이어지는 가장 단순한 서버 흐름을 이해한다.

```text
서버 열기 -> 클라이언트 접속 받기 -> 클라이언트가 보낸 줄 읽기 -> 그대로 다시 보내기 -> 연결 닫기
```

추천 구현 순서:

1. `echoserver_todo.c`를 연다.
2. `main` 함수 안의 TODO를 채운다.
3. `echo` 함수 안의 TODO를 채운다.
4. `make server`로 서버를 빌드한다.
5. 서버를 실행한다: `./echoserver 8000`
6. 다른 터미널에서 `nc` 또는 `telnet`으로 접속한다.

```bash
nc localhost 8000
```

그 다음 아무 문장이나 입력해봐.
서버가 네가 보낸 줄을 그대로 다시 보내주면 성공이야.

선택 연습:

1. `echoclient_todo.c`도 채운다.
2. `make client`로 클라이언트를 빌드한다.
3. 실행한다: `./echoclient localhost 8000`

핵심 감각:

```text
listenfd = 서버 문
connfd   = 한 클라이언트와 대화하는 전용 통로
rio_t    = connfd에서 줄 단위로 읽기 쉽게 도와주는 읽기 상태 구조체
buf      = 실제로 한 줄이 복사되어 들어오는 문자 배열
```
