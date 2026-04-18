/* $begin tinymain */
/*
 * tiny.c - A simple, iterative HTTP/1.0 Web server that uses the
 *     GET method to serve static and dynamic content.
 *
 * Updated 11/2019 droh
 *   - Fixed sprintf() aliasing issue in serve_static(), and clienterror().
 */
/*
Tiny 서버가 요청이 올 때마다 실행되는 건지, 계속 실행 중인 건지
clientaddr와 clientlen이 Accept 안에서 채워져 main에도 반영되는지, 그리고 Accept가 무엇을 리턴하는지
프록시가 아직 HTTP 요청을 안 보냈으면 doit 안에서 대기하는지
doit이나 Rio_readinitb 자체가 블로킹 함수인지
10분에 접속하고 15분 동안 요청을 안 보내면 Rio_readlineb가 계속 대기하는지, 아니면 리턴하고 반복문을 도는지
Rio_readlineb(&rio, buf, MAXLINE)가 사실상 블로킹 함수인지
fd에는 무슨 정보가 들어있는지
listenfd = Open_listenfd(argv[1])가 포트 8000 정보를 가진 fd 번호를 리턴하는 것인지
Getnameinfo에서 hostname, port에 주소값을 안 넘겼는데 어떻게 값이 채워지는
fd 0, fd 1, fd 2가 무엇이고 왜 기본으로 생기는지
10명이 들어오면 fd가 4, 5, 6처럼 순차적으로 생겨 관리되는지
지금 Tiny는 동시 처리가 안 되고 한 명만 가능한지
Tiny는 그냥 순차로 둬도 되는지, 여기서 동시성을 처리하면 안 되는지
프록시가 없다면 Tiny 서버가 동시성을 처리해야 하는지
프록시가 있으면 요청 10개에 Tiny 10개가 실행되는지
프록시는 여러 서버로 보낼 때 쓰는 것이고, 한 Tiny는 결국 하나의 요청만 처리하는지
프록시가 Tiny로 여러 요청을 보내면 Tiny가 순차 처리라 느릴 수 있는지
과제를 생각하지 않으면 Tiny 서버도 fd를 여러 개 생성해서 동시 처리하게 하면 좋은지
Rio_readinitb(&rio, fd)가 무슨 행위를 하는지
Rio_readinitb는 초기화만 하고, 실제 요청 전까지 Rio_readlineb에서 블로킹되며, 요청이 오면 배열에 저장되는지
"GET /home.html HTTP/1.0\r\nHost: localhost\r\n\r\n" 요청이 오면 rio_buf에 통째로 저장되고, buf에는 \n 기준으로 한 줄만 저장되는지
MAXLINE은 큰 수인데 실제 바이트 수만큼 어떻게 계산되어 실행되는지
MAXLINE은 최대 범위일 뿐이고, 바이트 수는 내부에서 따로 세는지
요청이 MAXLINE을 넘어가면 오류가 나는지, 아니면 다음 호출에서 이어서 저장되는지
Rio_readlineb함수는 Http요청의 request line부분만 읽어와서 저장 request line은 약속상 3개 그럼 4개거나 2개면 어떻게 하나
read_requesthdrs로 왜 끝까지 읽어야 하는지
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
    printf("Closeed connection from (%s, %s)\n", hostname, port);
    Close(connfd); // line:netp:tiny:close
  }
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

void clienterror(int fd, char *cause, char *errnum,
                 char *shortmsg, char *longmsg)
{
    char buf[MAXLINE];
    char body[MAXBUF];

    /*
     * clienterror의 목적:
     *
     * 클라이언트에게 "에러가 났다"는 HTTP 응답을 직접 만들어서 보내는 함수.
     *
     * 예를 들어 doit에서 이런 식으로 호출할 수 있다.
     *
     *   clienterror(fd, method, "501", "Not Implemented",
     *               "Tiny does not implement this method");
     *
     * 이 함수가 보내야 하는 응답의 전체 모양:
     *
     *   HTTP/1.0 501 Not Implemented\r\n
     *   Content-type: text/html\r\n
     *   Content-length: 바디길이\r\n
     *   \r\n
     *   <html>...</html>
     *
     * 중요한 점:
     * - fd는 stderr가 아니다.
     * - fd는 클라이언트와 연결된 connfd다.
     * - 따라서 printf가 아니라 Rio_writen(fd, ...)으로 클라이언트에게 보낸다.
     */

    /*
     * 미리 선언된 변수:
     *
     *   buf:
     *     HTTP 상태줄, Content-type, Content-length 같은 헤더 한 줄을 담는 용도
     *
     *   body:
     *     브라우저 화면에 보일 HTML 에러 페이지 전체를 담는 용도
     */

    /*
     * TODO 1. body에 HTML 에러 페이지를 만든다.
     *
     * body에는 이런 내용이 들어가면 된다.
     *
     *   <html><title>Tiny Error</title>
     *   <body bgcolor="ffffff">
     *   404: Not found
     *   <p>Tiny couldn't find this file: ./no-such-file.html
     *   <hr><em>The Tiny Web server</em>
     *
     * 사용할 인자:
     *   errnum   -> "404", "501", "403" 같은 상태 코드
     *   shortmsg -> "Not found", "Not Implemented", "Forbidden" 같은 짧은 설명
     *   longmsg  -> 자세한 설명
     *   cause    -> 에러 원인이 된 문자열
     *
     * 힌트:
     *   sprintf(body, ...);
     *
     * 주의:
     *   body에 여러 줄을 이어 붙일 때는
     *   sprintf(body, "%s추가내용", body);
     *   방식보다, 필요하면 책 코드처럼 한 줄씩 따라 작성해도 된다.
     */
    sprintf(body,
        "<html><title>Tiny Error</title>"
        "<body bgcolor=\"ffffff\">\r\n"
        "%s: %s\r\n"
        "<p>%s: %s\r\n"
        "<hr><em>The Tiny Web server</em>\r\n",
        errnum, shortmsg, longmsg, cause);
    /*
     * TODO 2. HTTP response line을 보낸다.
     *
     * 모양:
     *   HTTP/1.0 <errnum> <shortmsg>\r\n
     *
     * 예:
     *   HTTP/1.0 404 Not found\r\n
     *
     * 힌트:
     *   sprintf(buf, "HTTP/1.0 %s %s\r\n", errnum, shortmsg);
     *   Rio_writen(fd, buf, strlen(buf));
     */
    sprintf(buf,"HTTP/1.0 %s %s\r\n",errnum,shortmsg);
    Rio_writen(fd,buf,strlen(buf));

    /*
     * TODO 3. Content-type 헤더를 보낸다.
     *
     * 에러 응답 본문은 HTML이므로 text/html로 보낸다.
     *
     * 모양:
     *   Content-type: text/html\r\n
     *
     * 힌트:
     *   sprintf(buf, "Content-type: text/html\r\n");
     *   Rio_writen(fd, buf, strlen(buf));
     */
    sprintf(buf, "Content-type: text/html\r\n");
    Rio_writen(fd,buf,strlen(buf));

    /*
     * TODO 4. Content-length 헤더를 보낸다.
     *
     * Content-length는 body의 바이트 길이다.
     *
     * 모양:
     *   Content-length: <strlen(body)>\r\n
     *
     * 그리고 HTTP 헤더가 끝났다는 뜻으로 빈 줄도 보내야 한다.
     * 그래서 마지막은 "\r\n\r\n" 형태가 된다.
     *
     * 힌트:
     *   sprintf(buf, "Content-length: %d\r\n\r\n", (int)strlen(body));
     *   Rio_writen(fd, buf, strlen(buf));
     *
     * 왜 빈 줄이 필요할까?
     *   HTTP에서는 헤더와 본문 사이를 빈 줄로 구분하기 때문이다.
     */
    sprintf(buf, "Content-length: %d\r\n\r\n",strlen(body));
    Rio_writen(fd,buf,strlen(buf));

    /*
     * TODO 5. 마지막으로 HTML body를 클라이언트에게 보낸다.
     *
     * 힌트:
     *   Rio_writen(fd, body, strlen(body));
     *
     * 여기까지 보내면 브라우저나 curl은
     * "아, 서버가 에러 페이지를 응답했구나"라고 이해한다.
     */
    Rio_writen(fd, body, strlen(body));
}

void read_requesthdrs(rio_t *rp)
{
    char buf[MAXLINE];

    /*
     * read_requesthdrs의 목적:
     *
     * HTTP request line은 doit에서 이미 읽었다.
     * 이 함수는 그 뒤에 따라오는 request header들을 빈 줄까지 읽어서 비우는 함수다.
     *
     * 예:
     *   GET /home.html HTTP/1.0\r\n
     *   Host: localhost:8000\r\n
     *   User-Agent: curl/...\r\n
     *   \r\n
     *
     * doit에서 첫 줄은 이미 읽었으므로,
     * 여기서는 Host부터 읽기 시작해서 "\r\n"만 있는 줄까지 읽으면 된다.
     *
     * Tiny는 헤더 내용을 거의 사용하지 않는다.
     * 하지만 끝까지 읽어야 "요청 하나가 여기서 끝났다"는 경계가 명확해진다.
     */

    /*
     * 미리 선언된 변수:
     *
     *   buf:
     *     클라이언트가 보낸 header 한 줄을 임시로 담는 배열
     */

    /*
     * TODO 1. 첫 번째 header line을 읽는다.
     *
     * 힌트:
     *   Rio_readlineb(rp, buf, MAXLINE);
     *
     * rp는 이미 doit에서 Rio_readinitb가 끝난 rio 구조체의 주소다.
     * 그래서 여기서는 다시 Rio_readinitb를 하지 않는다.
     */
    Rio_readlineb(rp, buf, MAXLINE);
    while (strcmp(buf, "\r\n"))
    {
        printf("%s",buf);
        Rio_readlineb(rp, buf, MAXLINE);
    }
    

    /*
     * TODO 2. 빈 줄 "\r\n"이 나올 때까지 반복해서 읽는다.
     *
     * HTTP header의 끝은 빈 줄이다.
     *
     * 비교 힌트:
     *   strcmp(buf, "\r\n")
     *
     * 의미:
     *   strcmp(buf, "\r\n") != 0 이면 아직 헤더가 남아있다는 뜻이다.
     */

    /*
     * TODO 3. 반복문 안에서 현재 header line을 출력해도 된다.
     *
     * 예:
     *   printf("%s", buf);
     *
     * 이것은 디버깅용이다.
     * 나중에 필요 없으면 지워도 된다.
     */

    /*
     * TODO 4. 반복문 안에서 다음 header line을 다시 읽는다.
     *
     * 마지막으로 "\r\n"만 있는 줄을 만나면 반복을 멈춘다.
     */
}

int parse_uri(char *uri, char *filename, char *cgiargs)
{
    char *ptr;

    /*
     * parse_uri의 목적:
     *
     * 클라이언트가 요청한 URI를 Tiny 서버가 실제로 사용할 수 있는 정보로 바꾼다.
     *
     * 정적 요청 예:
     *   uri      = "/home.html"
     *   filename = "./home.html"
     *   cgiargs  = ""
     *   return   = 1
     *
     * 동적 요청 예:
     *   uri      = "/cgi-bin/adder?1&2"
     *   filename = "./cgi-bin/adder"
     *   cgiargs  = "1&2"
     *   return   = 0
     *
     * 반환값:
     *   1 -> 정적 파일 요청
     *   0 -> 동적 CGI 요청
     */

    /*
     * 미리 선언된 변수:
     *
     *   ptr:
     *     URI 안에서 '?' 위치를 찾을 때 사용할 포인터
     */

    /*
     * TODO 1. uri 안에 "cgi-bin"이 있는지 확인한다.
     *
     * 힌트:
     *   strstr(uri, "cgi-bin")
     *
     * "cgi-bin"이 없으면 정적 파일 요청이다.
     * "cgi-bin"이 있으면 동적 CGI 요청이다.
     */
    if(strstr(uri,"cgi-bin")==NULL)
    /*
     * TODO 2. 정적 파일 요청 처리
     *
     * 해야 할 일:
     *   1. cgiargs를 빈 문자열로 만든다.
     *      힌트: strcpy(cgiargs, "");
     *
     *   2. filename이 "."으로 시작하게 만든다.
     *      예: uri "/home.html" -> filename "./home.html"
     *
     *   3. uri가 "/"로 끝나면 기본 파일 "home.html"을 붙인다.
     *      예: uri "/" -> filename "./home.html"
     *
     *   4. 정적 요청이므로 1을 return한다.
     */
    {
        strcpy(cgiargs,"");
        strcpy(filename,".");
        strcat(filename,uri);
        if(uri[strlen(uri)-1]=='/')strcat(filename,"home.html");
        return 1;
    }
    /*
     * TODO 3. 동적 CGI 요청 처리
     *
     * 해야 할 일:
     *   1. uri 안에서 '?' 위치를 찾는다.
     *      힌트: ptr = index(uri, '?');
     *
     *   2. '?'가 있으면:
     *      - '?' 뒤쪽 문자열을 cgiargs에 복사한다.
     *      - '?' 자리를 '\0'으로 바꿔서 uri를 filename 부분까지만 남긴다.
     *
     *      예:
     *        uri = "/cgi-bin/adder?1&2"
     *        cgiargs = "1&2"
     *        uri = "/cgi-bin/adder"
     *
     *   3. '?'가 없으면:
     *      - cgiargs를 빈 문자열로 만든다.
     *
     *   4. filename이 "."으로 시작하게 만든다.
     *      예: uri "/cgi-bin/adder" -> filename "./cgi-bin/adder"
     *
     *   5. 동적 요청이므로 0을 return한다.
     */
    else{
        ptr=strchr(uri,'?');
        if(ptr){
            strcpy(cgiargs,ptr+1);
            *ptr = '\0';
        }
        else strcpy(cgiargs,"");
        strcpy(filename,".");
        strcat(filename,uri);
        return 0;
    }
    /*
     * TODO 4. 모든 분기에서 return이 있어야 한다.
     *
     * 컴파일 경고를 피하려면 정적/동적 분기 안에서 반드시 return하자.
     */
}

void serve_static(int fd, char *filename, int filesize)
{
    int srcfd;
    char *srcp;
    char filetype[MAXLINE];
    char buf[MAXBUF];

    /*
     * serve_static의 목적:
     *
     * HTML, 이미지, CSS, JS 같은 정적 파일을 클라이언트에게 보내는 함수다.
     *
     * 흐름:
     *   1. 파일 확장자로 Content-type을 정한다.
     *   2. HTTP response header를 만든다.
     *   3. header를 클라이언트에게 보낸다.
     *   4. 파일을 연다.
     *   5. 파일 내용을 메모리에 매핑하거나 읽는다.
     *   6. 파일 바이트를 클라이언트에게 보낸다.
     *   7. 파일/메모리 자원을 정리한다.
     */

    /*
     * 미리 선언된 변수:
     *
     *   srcfd:
     *     정적 파일을 open했을 때 얻는 파일 fd
     *
     *   srcp:
     *     파일 내용을 메모리에 매핑했을 때 시작 주소
     *
     *   filetype:
     *     Content-type 값을 담을 배열
     *     예: "text/html", "image/jpeg"
     *
     *   buf:
     *     HTTP 응답 헤더를 만들 때 사용할 임시 버퍼
     */

    /*
     * TODO 1. filename의 확장자를 보고 filetype을 채운다.
     *
     * 힌트:
     *   get_filetype(filename, filetype);
     */

    /*
     * TODO 2. HTTP response header를 만든다.
     *
     * 응답 예:
     *   HTTP/1.0 200 OK\r\n
     *   Server: Tiny Web Server\r\n
     *   Connection: close\r\n
     *   Content-length: <filesize>\r\n
     *   Content-type: <filetype>\r\n
     *   \r\n
     *
     * header와 body 사이에는 반드시 빈 줄 "\r\n"이 있어야 한다.
     */

    /*
     * TODO 3. 만든 header를 fd로 보낸다.
     *
     * 힌트:
     *   Rio_writen(fd, buf, strlen(buf));
     *
     * 여기서 fd는 클라이언트와 연결된 connfd다.
     */

    /*
     * TODO 4. 정적 파일을 연다.
     *
     * 힌트:
     *   srcfd = Open(filename, O_RDONLY, 0);
     *
     * O_RDONLY는 읽기 전용으로 파일을 열겠다는 뜻이다.
     */

    /*
     * TODO 5. 파일 내용을 메모리에 매핑한다.
     *
     * 책 Tiny 예제 방식:
     *   srcp = Mmap(0, filesize, PROT_READ, MAP_PRIVATE, srcfd, 0);
     *
     * 의미:
     *   파일 내용을 srcp가 가리키는 메모리 영역처럼 다룰 수 있게 한다.
     */

    /*
     * TODO 6. 파일 fd는 더 이상 필요 없으므로 닫는다.
     *
     * 힌트:
     *   Close(srcfd);
     *
     * mmap을 했다면 파일 fd를 닫아도 매핑된 메모리 srcp는 아직 사용할 수 있다.
     */

    /*
     * TODO 7. 파일 내용을 클라이언트에게 보낸다.
     *
     * 힌트:
     *   Rio_writen(fd, srcp, filesize);
     *
     * 여기서는 strlen을 쓰면 안 된다.
     * 이미지 같은 바이너리 파일은 중간에 '\0' 바이트가 있을 수 있기 때문이다.
     * 파일 크기인 filesize만큼 정확히 보내야 한다.
     */

    /*
     * TODO 8. 매핑한 메모리를 해제한다.
     *
     * 힌트:
     *   Munmap(srcp, filesize);
     */
}

void get_filetype(char *filename, char *filetype)
{
    /*
     * get_filetype의 목적:
     *
     * 파일 이름의 확장자를 보고 HTTP Content-type을 정한다.
     *
     * 예:
     *   home.html  -> text/html
     *   image.gif  -> image/gif
     *   photo.jpg  -> image/jpeg
     *   movie.mp4  -> video/mp4
     *   그 외      -> text/plain
     *
     * filetype은 호출한 쪽에서 넘겨준 배열이다.
     * 이 함수는 그 배열 안에 문자열을 써준다.
     */

    /*
     * TODO 1. filename 안에 ".html"이 있는지 확인한다.
     *
     * 힌트:
     *   strstr(filename, ".html")
     *
     * 있으면:
     *   strcpy(filetype, "text/html");
     */

    /*
     * TODO 2. ".gif"이면 "image/gif"로 설정한다.
     */

    /*
     * TODO 3. ".png"이면 "image/png"로 설정한다.
     */

    /*
     * TODO 4. ".jpg"이면 "image/jpeg"로 설정한다.
     *
     * 필요하면 ".jpeg"도 같이 처리할 수 있다.
     */

    /*
     * TODO 5. ".mp4"이면 "video/mp4"로 설정한다.
     */

    /*
     * TODO 6. 위 확장자에 해당하지 않으면 기본값을 넣는다.
     *
     * 힌트:
     *   strcpy(filetype, "text/plain");
     */
}

void serve_dynamic(int fd, char *filename, char *cgiargs)
{
    char buf[MAXLINE];
    char *emptylist[] = { NULL };

    /*
     * serve_dynamic의 목적:
     *
     * CGI 프로그램을 실행해서 그 출력 결과를 클라이언트에게 보내는 함수다.
     *
     * 예:
     *   요청 URI:
     *     /cgi-bin/adder?1&2
     *
     * parse_uri 결과:
     *   filename = "./cgi-bin/adder"
     *   cgiargs  = "1&2"
     *
     * serve_dynamic은 adder 프로그램을 자식 프로세스에서 실행한다.
     * adder의 stdout을 클라이언트 fd로 연결해두면,
     * adder가 printf한 내용이 브라우저/curl로 바로 나간다.
     */

    /*
     * 미리 선언된 변수:
     *
     *   buf:
     *     먼저 보낼 HTTP header를 담는 임시 버퍼
     *
     *   emptylist:
     *     Execve에 넘길 argv 배열
     *     여기서는 별도 인자 없이 CGI 프로그램을 실행하기 위해 NULL만 둔다.
     */

    /*
     * TODO 1. 클라이언트에게 기본 HTTP response header를 먼저 보낸다.
     *
     * 동적 응답의 body는 CGI 프로그램이 직접 출력한다.
     * Tiny는 먼저 최소한의 헤더만 보낸다.
     *
     * 예:
     *   HTTP/1.0 200 OK\r\n
     *   Server: Tiny Web Server\r\n
     *
     * 힌트:
     *   sprintf(buf, "HTTP/1.0 200 OK\r\n");
     *   Rio_writen(fd, buf, strlen(buf));
     *
     *   sprintf(buf, "Server: Tiny Web Server\r\n");
     *   Rio_writen(fd, buf, strlen(buf));
     */

    /*
     * TODO 2. Fork로 자식 프로세스를 만든다.
     *
     * 힌트:
     *   if (Fork() == 0) {
     *       ...
     *   }
     *
     * 부모 프로세스:
     *   자식이 끝날 때까지 Wait(NULL) 한다.
     *
     * 자식 프로세스:
     *   CGI 프로그램을 실행한다.
     */

    /*
     * TODO 3. 자식 프로세스 안에서 QUERY_STRING 환경변수를 설정한다.
     *
     * CGI 프로그램은 직접 인자로 "1&2"를 받는 게 아니라,
     * 환경변수 QUERY_STRING에서 query string을 읽는다.
     *
     * 힌트:
     *   setenv("QUERY_STRING", cgiargs, 1);
     *
     * 예:
     *   cgiargs = "1&2"
     *   getenv("QUERY_STRING") -> "1&2"
     */

    /*
     * TODO 4. 자식 프로세스 안에서 stdout을 클라이언트 fd로 바꾼다.
     *
     * 힌트:
     *   Dup2(fd, STDOUT_FILENO);
     *
     * 의미:
     *   이후 자식 프로세스가 printf로 출력하는 내용은
     *   터미널이 아니라 클라이언트 소켓으로 나간다.
     */

    /*
     * TODO 5. 자식 프로세스 안에서 CGI 프로그램을 실행한다.
     *
     * 힌트:
     *   Execve(filename, emptylist, environ);
     *
     * filename:
     *   "./cgi-bin/adder"
     *
     * emptylist:
     *   argv 역할. 여기서는 인자를 넘기지 않으므로 NULL 배열.
     *
     * environ:
     *   현재 환경변수 목록. QUERY_STRING도 여기에 포함된다.
     */

    /*
     * TODO 6. 부모 프로세스는 자식이 끝날 때까지 기다린다.
     *
     * 힌트:
     *   Wait(NULL);
     *
     * 기다리지 않으면 좀비 프로세스가 생길 수 있다.
     */
}
