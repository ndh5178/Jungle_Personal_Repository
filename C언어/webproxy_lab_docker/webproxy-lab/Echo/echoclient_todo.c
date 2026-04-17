#include "csapp.h"

/*
 * 선택 연습용 Echo 클라이언트.
 *
 * 이 클라이언트가 하는 일:
 * - Echo 서버에 접속한다.
 * - 키보드에서 한 줄을 입력받는다.
 * - 그 줄을 서버로 보낸다.
 * - 서버가 그대로 돌려준 줄을 읽는다.
 * - 화면에 출력한다.
 */

int main(int argc, char **argv)
{
    int clientfd;
    char *host;
    char *port;
    char buf[MAXLINE];
    rio_t rio;

    /*
     * TODO 1. 명령행 인자 개수를 확인한다.
     *
     * 기대하는 실행 명령:
     *   ./echoclient <host> <port>
     *
     * 예시:
     *   ./echoclient localhost 8000
     *
     * argc가 3이 아니면:
     *   - stderr에 사용법을 출력한다.
     *   - exit으로 프로그램을 종료한다.
     */

    /*
     * TODO 2. argv[1]과 argv[2]를 변수에 저장한다.
     *
     * host = argv[1]
     * port = argv[2]
     */

    /*
     * TODO 3. 서버에 접속한다.
     *
     * Open_clientfd(host, port)를 사용한다.
     *
     * 결과:
     *   clientfd는 서버와 연결된 소켓 번호다.
     *
     * 서버 입장에서는 이 연결이 Accept를 통해 connfd로 보인다.
     */

    /*
     * TODO 4. clientfd에 대한 RIO 버퍼를 초기화한다.
     *
     * Rio_readinitb(&rio, clientfd)를 사용한다.
     */

    /*
     * TODO 5. 키보드 입력을 한 줄씩 반복해서 처리한다.
     *
     * 아래 형태를 사용한다.
     *   while (Fgets(buf, MAXLINE, stdin) != NULL) {
     *       ...
     *   }
     *
     * 반복문 한 번마다 할 일:
     *   1. Rio_writen(clientfd, buf, strlen(buf))로 서버에 buf를 보낸다.
     *   2. Rio_readlineb(&rio, buf, MAXLINE)으로 서버가 돌려준 한 줄을 읽는다.
     *   3. Fputs(buf, stdout)으로 화면에 출력한다.
     */

    /*
     * TODO 6. 프로그램을 끝내기 전에 clientfd를 닫는다.
     */

    return 0;
}
