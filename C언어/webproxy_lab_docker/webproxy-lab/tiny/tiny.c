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
    sprintf(body,
        "<html><title>Tiny Error</title>"
        "<body bgcolor=\"ffffff\">\r\n"
        "%s: %s\r\n"
        "<p>%s: %s\r\n"
        "<hr><em>The Tiny Web server</em>\r\n",
        errnum, shortmsg, longmsg, cause);
    sprintf(buf,"HTTP/1.0 %s %s\r\n",errnum,shortmsg);
    Rio_writen(fd,buf,strlen(buf));
    sprintf(buf, "Content-type: text/html\r\n");
    Rio_writen(fd,buf,strlen(buf));
    sprintf(buf, "Content-length: %d\r\n\r\n",strlen(body));
    Rio_writen(fd,buf,strlen(buf));
    Rio_writen(fd, body, strlen(body));
}

void read_requesthdrs(rio_t *rp)
{
    char buf[MAXLINE];
    Rio_readlineb(rp, buf, MAXLINE);
    while (strcmp(buf, "\r\n"))
    {
        printf("%s",buf);
        Rio_readlineb(rp, buf, MAXLINE);
    }
}

int parse_uri(char *uri, char *filename, char *cgiargs)
{
    char igzist,*ptr=&igzist;
    if(strstr(uri,"cgi-bin")==NULL)strcpy(cgiargs,"");
    else
    {
        ptr=strchr(uri,'?');
        if(ptr){
            strcpy(cgiargs,ptr+1);
            *ptr = '\0';
        }
        else strcpy(cgiargs,"");
    }
    strcpy(filename,".");
    strcat(filename,uri);
    if(ptr==&igzist){
        if(uri[strlen(uri)-1]=='/')strcat(filename,"home.html");
        return 1;
    }
    else return 0;
}

void serve_static(int fd, char *filename, int filesize)
{
    int srcfd;
    char *srcp;
    char filetype[MAXLINE];
    char buf[MAXBUF];
    get_filetype(filename,filetype);
    sprintf(buf,"HTTP/1.0 200 OK\r\n"
        "Server: Tiny Web Server\r\n"
        "Connection: close\r\n"
        "Content-length: %d\r\n"
        "Content-type: %s\r\n"
        "\r\n",filesize,filetype);
    Rio_writen(fd,buf,strlen(buf));
    srcfd = Open(filename, O_RDONLY, 0);
    srcp = Mmap(0, filesize, PROT_READ, MAP_PRIVATE, srcfd, 0);
    Close(srcfd);
    Rio_writen(fd, srcp, filesize);
    Munmap(srcp, filesize);
}

void get_filetype(char *filename, char *filetype)
{
    if(strstr(filename,".html"))strcpy(filetype,"text/html");
    else if(strstr(filename,".gif"))strcpy(filetype,"image/gif");
    else if(strstr(filename,".png"))strcpy(filetype,"image/png");
    else if(strstr(filename, ".jpg") || strstr(filename,".jpeg"))strcpy(filetype,"image/jpeg");
    else if(strstr(filename,".mp4"))strcpy(filetype,"video/mp4");
    else strcpy(filetype, "text/plain");
}

void serve_dynamic(int fd, char *filename, char *cgiargs)
{
    char buf[MAXLINE];
    char *emptylist[] = { NULL };
    sprintf(buf,"HTTP/1.0 200 OK\r\n"
        "Server: Tiny Web Server\r\n"
    );
    Rio_writen(fd, buf, strlen(buf));
    if(Fork()==0)
    {
        setenv("QUERY_STRING", cgiargs, 1);
        Dup2(fd, STDOUT_FILENO);
        Execve(filename, emptylist, environ);
    }
    Wait(NULL);
}
