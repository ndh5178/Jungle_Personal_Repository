/* $begin tinymain */
/*
 * tiny.c - A simple, iterative HTTP/1.0 Web server that uses the
 *     GET method to serve static and dynamic content.
 *
 * Updated 11/2019 droh
 *   - Fixed sprintf() aliasing issue in serve_static(), and clienterror().
 */
#include "csapp.h"

void doit(int fd);
void read_requesthdrs(rio_t *rp);
int parse_uri(char *uri, char *filename, char *cgiargs);
void serve_static(int fd, char *filename, int filesize);
void get_filetype(char *filename, char *filetype);
void serve_dynamic(int fd, char *filename, char *cgiargs);
void clienterror(int fd, char *cause, char *errnum, char *shortmsg,
                 char *longmsg);

int main(int argc, char **argv)
{
  int listenfd, connfd;
  char hostname[MAXLINE], port[MAXLINE];
  socklen_t clientlen;
  struct sockaddr_storage clientaddr;

  /* Check command line args */
  if (argc != 2)
  {
    fprintf(stderr, "usage: %s <port>\n", argv[0]);
    exit(1);
  }

  listenfd = Open_listenfd(argv[1]);
  while (1)
  {
    clientlen = sizeof(clientaddr);
    connfd = Accept(listenfd, (SA *)&clientaddr,&clientlen); // line:netp:tiny:accept
    Getnameinfo((SA *)&clientaddr, clientlen, &hostname[0], MAXLINE, &port[0], MAXLINE,0);
    printf("Accepted connection from (%s, %s)\n", hostname, port);
    doit(connfd);  // line:netp:tiny:doit
    Close(connfd); // line:netp:tiny:close
  }
}

void doit(int fd)
{
    /*
     * TODO 1. 필요한 변수 선언
     * - is_static: 정적 파일인지 동적 CGI인지 구분
     * - struct stat sbuf: 파일 상태 정보 저장
     * - buf: 요청 한 줄 또는 헤더 한 줄을 담을 버퍼
     * - method, uri, version: request line 파싱 결과
     * - filename: 실제 파일 경로
     * - cgiargs: CGI query string
     * - rio_t rio: fd에서 안전하게 읽기 위한 RIO 버퍼
     */

    /*
     * TODO 2. RIO 버퍼 초기화
     * - Rio_readinitb(&rio, fd);
     * - fd는 클라이언트와 연결된 connfd
     */

    /*
     * TODO 3. HTTP request line 읽기
     * - Rio_readlineb(&rio, buf, MAXLINE);
     * - 예: "GET /home.html HTTP/1.0\r\n"
     * - 클라이언트가 연결만 하고 끊으면 0이 올 수 있으므로 return 처리
     */

    /*
     * TODO 4. request line 파싱
     * - sscanf(buf, "%s %s %s", method, uri, version);
     * - method = "GET"
     * - uri = "/home.html"
     * - version = "HTTP/1.0"
     */

    /*
     * TODO 5. method가 GET인지 확인
     * - Tiny는 GET만 처리
     * - GET이 아니면 clienterror로 501 Not Implemented 응답 후 return
     */

    /*
     * TODO 6. 요청 헤더 끝까지 읽기
     * - read_requesthdrs(&rio);
     * - Tiny에서는 헤더 내용을 거의 쓰지 않지만,
     *   빈 줄 "\r\n"이 나올 때까지 읽어야 다음 처리가 깨지지 않음
     */

    /*
     * TODO 7. URI 파싱
     * - is_static = parse_uri(uri, filename, cgiargs);
     * - 정적 파일이면 filename 예: "./home.html"
     * - 동적 CGI면 filename 예: "./cgi-bin/adder", cgiargs 예: "1&2"
     */

    /*
     * TODO 8. 파일 존재 여부 확인
     * - stat(filename, &sbuf)
     * - 실패하면 404 Not Found 응답 후 return
     */

    /*
     * TODO 9. 정적 파일 처리
     * - is_static == 1인 경우
     * - 일반 파일인지 확인: S_ISREG(sbuf.st_mode)
     * - 읽기 권한이 있는지 확인: S_IRUSR & sbuf.st_mode
     * - 조건 실패 시 403 Forbidden
     * - 성공 시 serve_static(fd, filename, sbuf.st_size);
     */

    /*
     * TODO 10. 동적 CGI 처리
     * - is_static == 0인 경우
     * - 일반 파일인지 확인: S_ISREG(sbuf.st_mode)
     * - 실행 권한이 있는지 확인: S_IXUSR & sbuf.st_mode
     * - 조건 실패 시 403 Forbidden
     * - 성공 시 serve_dynamic(fd, filename, cgiargs);
     */

    /*
     * TODO 11. 함수 종료
     * - 여기서 fd를 Close하지 않음
     * - fd는 main에서 doit(connfd) 이후 Close(connfd)로 닫음
     */
}

void doit(int fd)
{
    int is_static;
    struct stat sbuf;
    char buf[MAXLINE], method[MAXLINE], uri[MAXLINE], version[MAXLINE];
    char filename[MAXLINE], cgiargs[MAXLINE];
    rio_t rio;

    /* TODO 1. fd에 RIO 버퍼 연결 */
    Rio_readinitb(&rio, fd);

    /* TODO 2. HTTP 요청 첫 줄 읽기 */
    if (!Rio_readlineb(&rio, buf, MAXLINE))
        return;

    /* TODO 3. request line 출력 또는 디버깅 */
    printf("%s", buf);

    /* TODO 4. method, uri, version 파싱 */
    sscanf(buf, "%s %s %s", method, uri, version);

    /* TODO 5. GET 메서드만 허용 */
    if (strcasecmp(method, "GET")) {
        clienterror(fd, method, "501", "Not Implemented",
                    "Tiny does not implement this method");
        return;
    }

    /* TODO 6. 나머지 request headers 읽기 */
    read_requesthdrs(&rio);

    /* TODO 7. URI를 filename과 cgiargs로 변환 */
    is_static = parse_uri(uri, filename, cgiargs);

    /* TODO 8. 파일이 존재하는지 확인 */
    if (stat(filename, &sbuf) < 0) {
        clienterror(fd, filename, "404", "Not found",
                    "Tiny couldn't find this file");
        return;
    }

    /* TODO 9. 정적 파일이면 파일 내용을 응답 */
    if (is_static) {
        if (!(S_ISREG(sbuf.st_mode)) || !(S_IRUSR & sbuf.st_mode)) {
            clienterror(fd, filename, "403", "Forbidden",
                        "Tiny couldn't read the file");
            return;
        }

        serve_static(fd, filename, sbuf.st_size);
    }

    /* TODO 10. 동적 CGI면 프로그램 실행 결과를 응답 */
    else {
        if (!(S_ISREG(sbuf.st_mode)) || !(S_IXUSR & sbuf.st_mode)) {
            clienterror(fd, filename, "403", "Forbidden",
                        "Tiny couldn't run the CGI program");
            return;
        }

        serve_dynamic(fd, filename, cgiargs);
    }
}
