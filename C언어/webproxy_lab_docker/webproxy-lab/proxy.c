#include <stdio.h>
#include "csapp.h"
/* Recommended max cache and object sizes */
#define MAX_CACHE_SIZE 1049000
#define MAX_OBJECT_SIZE 102400

/* You won't lose style points for including this long line in your code */
static const char *user_agent_hdr =
    "User-Agent: Mozilla/5.0 (X11; Linux x86_64; rv:10.0.3) Gecko/20120305 "
    "Firefox/10.0.3\r\n";

void doit(int connfd);
void parse_uri(char *uri, char *host, char *port, char *path);
void build_http_header(char *http_header, char *host, char *port, char *path,
                       rio_t *client_rio);

int main(int argc, char **argv)
{
  int listenfd,connectfd;
  char hostname[MAXLINE], port[MAXLINE];
  socklen_t clientlen;
  struct sockaddr_storage clientaddr;
  if(argc!=2){
    fprintf(stderr, "usage: %s <port>\n", argv[0]);
    exit(1);
  }
  listenfd=open_listenfd(argv[1]);
  while (1)
  {
    clientlen = sizeof(clientaddr);
    connectfd=Accept(listenfd, (SA *)&clientaddr,&clientlen);
    Getnameinfo((SA *)&clientaddr, clientlen, &hostname[0], MAXLINE, &port[0], MAXLINE,0);
    printf("Accepted connection from (%s, %s)\n", hostname, port);
    doit(connectfd);
    printf("Closeed connection from (%s, %s)\n", hostname, port);
    Close(connectfd);
  }
  return 0;
}

void doit(int connfd)
{
  int serverfd;
  ssize_t n;
  char buf[MAXLINE];
  char method[MAXLINE], uri[MAXLINE], version[MAXLINE];
  char host[MAXLINE], port[MAXLINE], path[MAXLINE];
  char http_header[MAXBUF];
  rio_t client_rio;
  rio_t server_rio;

  /*
   * doit의 목적:
   *
   * 클라이언트가 프록시에 보낸 HTTP 요청 하나를 처리한다.
   *
   * Tiny의 doit은:
   *   클라이언트 요청 -> 내 파일 시스템에서 파일 찾기 -> 직접 응답
   *
   * Proxy의 doit은:
   *   클라이언트 요청 -> 실제 웹서버에 대신 요청 -> 받은 응답을 클라이언트에게 전달
   *
   * 그래서 프록시의 doit은 서버 역할과 클라이언트 역할을 둘 다 한다.
   */

  /*
   * 미리 선언된 변수:
   *
   *   connfd:
   *     브라우저/curl 같은 클라이언트와 연결된 fd
   *
   *   serverfd:
   *     프록시가 실제 웹서버(origin server)에 연결할 때 생기는 fd
   *
   *   client_rio:
   *     connfd에서 클라이언트 요청을 읽기 위한 RIO 상태
   *
   *   server_rio:
   *     serverfd에서 실제 웹서버 응답을 읽기 위한 RIO 상태
   *
   *   method, uri, version:
   *     클라이언트 request line을 파싱한 결과
   *
   *   host, port, path:
   *     uri를 프록시가 origin server에 접속할 수 있게 나눈 결과
   *
   *   http_header:
   *     origin server에 보낼 새 HTTP 요청 전체
   *
   *   buf:
   *     요청/응답 데이터를 임시로 담는 버퍼
   */

  /*
   * TODO 1. connfd에 대한 RIO 버퍼를 초기화한다.
   *
   * 힌트:
   *   Rio_readinitb(&client_rio, connfd);
   *
   * 아직 데이터를 읽는 단계가 아니라,
   * client_rio가 connfd에서 읽을 준비를 하게 만드는 단계다.
   */

  /*
   * TODO 2. 클라이언트의 request line을 한 줄 읽는다.
   *
   * 예:
   *   GET http://localhost:8000/home.html HTTP/1.1\r\n
   *
   * 힌트:
   *   if (!Rio_readlineb(&client_rio, buf, MAXLINE))
   *       return;
   */

  /*
   * TODO 3. request line을 method, uri, version으로 파싱한다.
   *
   * 힌트:
   *   sscanf(buf, "%s %s %s", method, uri, version);
   *
   * 결과 예:
   *   method  = "GET"
   *   uri     = "http://localhost:8000/home.html"
   *   version = "HTTP/1.1"
   */

  /*
   * TODO 4. GET 요청만 처리한다.
   *
   * Basic 프록시는 우선 GET만 처리해도 된다.
   * GET이 아니면 에러 응답을 만들거나, 일단 return해도 된다.
   *
   * 힌트:
   *   if (strcasecmp(method, "GET"))
   *       return;
   */

  /*
   * TODO 5. URI를 host, port, path로 나눈다.
   *
   * 힌트:
   *   parse_uri(uri, host, port, path);
   *
   * 예:
   *   uri  = "http://localhost:8000/home.html"
   *   host = "localhost"
   *   port = "8000"
   *   path = "/home.html"
   */

  /*
   * TODO 6. 클라이언트가 보낸 헤더를 읽고,
   *         origin server에 보낼 새 HTTP 요청을 만든다.
   *
   * 힌트:
   *   build_http_header(http_header, host, port, path, &client_rio);
   *
   * 주의:
   *   build_http_header 안에서 클라이언트의 나머지 request header를
   *   빈 줄 "\r\n"까지 읽어야 한다.
   */

  /*
   * TODO 7. origin server에 연결한다.
   *
   * 힌트:
   *   serverfd = Open_clientfd(host, port);
   *
   * 이 순간 프록시 프로세스 안에는 보통 이런 fd 구조가 된다.
   *
   *   listenfd -> 프록시 접속 대기
   *   connfd   -> 클라이언트와 연결
   *   serverfd -> 실제 웹서버와 연결
   */

  /*
   * TODO 8. origin server에 새 HTTP 요청을 보낸다.
   *
   * 힌트:
   *   Rio_writen(serverfd, http_header, strlen(http_header));
   */

  /*
   * TODO 9. serverfd에 대한 RIO 버퍼를 초기화한다.
   *
   * 힌트:
   *   Rio_readinitb(&server_rio, serverfd);
   */

  /*
   * TODO 10. origin server 응답을 읽어서 클라이언트에게 그대로 전달한다.
   *
   * 힌트:
   *   while ((n = Rio_readnb(&server_rio, buf, MAXBUF)) > 0) {
   *       Rio_writen(connfd, buf, n);
   *   }
   *
   * 여기서는 Rio_readlineb보다 Rio_readnb가 더 자연스럽다.
   * 이유:
   *   서버 응답 body에는 이미지나 실행 파일 같은 바이너리 데이터가 올 수 있고,
   *   줄 단위 문자열이라고 가정하면 안 되기 때문이다.
   */

  /*
   * TODO 11. origin server와 연결된 serverfd를 닫는다.
   *
   * 힌트:
   *   Close(serverfd);
   *
   * connfd는 main에서 닫을지, doit에서 닫을지 한 군데로 정하면 된다.
   * Tiny처럼 main에서 Close(connfd) 하는 구조가 처음에는 이해하기 쉽다.
   */
}

void parse_uri(char *uri, char *host, char *port, char *path)
{
  char *host_begin;
  char *host_end;
  char *port_begin;
  char *path_begin;
  int host_len;
  int port_len;

  /*
   * parse_uri의 목적:
   *
   * 클라이언트가 프록시에 보낸 absolute URI를
   * 실제 origin server에 접속하기 좋은 정보로 나눈다.
   *
   * 예:
   *   uri  = "http://localhost:8000/home.html"
   *
   * 결과:
   *   host = "localhost"
   *   port = "8000"
   *   path = "/home.html"
   *
   * 포트가 없는 경우:
   *   uri  = "http://example.com/index.html"
   *
   * 결과:
   *   host = "example.com"
   *   port = "80"
   *   path = "/index.html"
   */

  /*
   * 미리 선언된 변수:
   *
   *   host_begin:
   *     "http://" 뒤에서 host가 시작하는 위치
   *
   *   host_end:
   *     host가 끝나는 위치. ':' 또는 '/' 위치가 될 수 있다.
   *
   *   port_begin:
   *     ':' 뒤에서 port가 시작하는 위치
   *
   *   path_begin:
   *     path가 시작하는 '/' 위치
   *
   *   host_len, port_len:
   *     strncpy로 host/port를 복사할 때 사용할 길이
   */

  /*
   * TODO 1. uri가 "http://"로 시작하면 그 뒤를 host_begin으로 잡는다.
   *
   * 힌트:
   *   if (strstr(uri, "http://") == uri)
   *       host_begin = uri + strlen("http://");
   *   else
   *       host_begin = uri;
   *
   * 프록시 요청에서는 보통 absolute URI가 오지만,
   * 방어적으로 "http://"가 없는 경우도 생각할 수 있다.
   */

  /*
   * TODO 2. path가 시작되는 '/'를 찾는다.
   *
   * 힌트:
   *   path_begin = strchr(host_begin, '/');
   *
   * 예:
   *   host_begin = "localhost:8000/home.html"
   *   path_begin = "/home.html"의 '/' 위치
   *
   * 만약 '/'가 없으면 path는 "/"로 두면 된다.
   */

  /*
   * TODO 3. port 구분자인 ':'를 찾는다.
   *
   * 힌트:
   *   port_begin = strchr(host_begin, ':');
   *
   * 단, ':'가 path 뒤에 있으면 host:port 구분자가 아니다.
   * Basic에서는 보통 "http://host:port/path" 형태를 먼저 처리하면 된다.
   */

  /*
   * TODO 4. port가 있는 경우 host와 port를 분리한다.
   *
   * 예:
   *   host_begin = "localhost:8000/home.html"
   *   port_begin = ":8000/home.html"
   *   path_begin = "/home.html"
   *
   * 결과:
   *   host = "localhost"
   *   port = "8000"
   *   path = "/home.html"
   *
   * 힌트:
   *   host_end = port_begin;
   *   host_len = host_end - host_begin;
   *   strncpy(host, host_begin, host_len);
   *   host[host_len] = '\0';
   *
   *   port_begin은 ':'를 가리키므로 실제 포트 시작은 port_begin + 1이다.
   */

  /*
   * TODO 5. port가 없는 경우 기본 포트 "80"을 사용한다.
   *
   * 예:
   *   uri = "http://example.com/index.html"
   *
   * 결과:
   *   host = "example.com"
   *   port = "80"
   *   path = "/index.html"
   */

  /*
   * TODO 6. path를 복사한다.
   *
   * path_begin이 있으면:
   *   strcpy(path, path_begin);
   *
   * path_begin이 없으면:
   *   strcpy(path, "/");
   *
   * origin server에 보내는 request line은 absolute URI가 아니라 path만 사용한다.
   *
   * 예:
   *   GET /home.html HTTP/1.0\r\n
   */
}

void build_http_header(char *http_header, char *host, char *port, char *path,
                       rio_t *client_rio)
{
  char buf[MAXLINE];
  char request_hdr[MAXLINE];
  char host_hdr[MAXLINE];
  char other_hdr[MAXBUF];

  /*
   * build_http_header의 목적:
   *
   * 클라이언트가 프록시에 보낸 HTTP 요청 헤더를 읽고,
   * origin server에 보낼 새로운 HTTP 요청을 만든다.
   *
   * 클라이언트 -> 프록시 요청 예:
   *   GET http://localhost:8000/home.html HTTP/1.1\r\n
   *   Host: localhost:8000\r\n
   *   User-Agent: curl/...\r\n
   *   Accept: */*\r\n
   *   \r\n
   *
   * 프록시 -> origin server 요청 예:
   *   GET /home.html HTTP/1.0\r\n
   *   Host: localhost:8000\r\n
   *   User-Agent: <Proxy Lab 고정 문자열>\r\n
   *   Connection: close\r\n
   *   Proxy-Connection: close\r\n
   *   Accept: */*\r\n
   *   \r\n
   */

  /*
   * 미리 선언된 변수:
   *
   *   buf:
   *     클라이언트가 보낸 header 한 줄을 읽을 임시 버퍼
   *
   *   request_hdr:
   *     "GET <path> HTTP/1.0\r\n" 요청줄
   *
   *   host_hdr:
   *     Host 헤더
   *
   *   other_hdr:
   *     클라이언트가 보낸 나머지 헤더 중 유지할 헤더들
   *
   *   http_header:
   *     최종적으로 origin server에 보낼 전체 HTTP 요청 문자열
   */

  /*
   * TODO 1. request line을 만든다.
   *
   * 프록시가 origin server에 보낼 때는 absolute URI가 아니라 path만 보낸다.
   *
   * 예:
   *   GET /home.html HTTP/1.0\r\n
   *
   * 힌트:
   *   sprintf(request_hdr, "GET %s HTTP/1.0\r\n", path);
   */

  /*
   * TODO 2. host_hdr와 other_hdr를 빈 문자열로 초기화한다.
   *
   * 힌트:
   *   strcpy(host_hdr, "");
   *   strcpy(other_hdr, "");
   */

  /*
   * TODO 3. 클라이언트가 보낸 request header를 빈 줄까지 읽는다.
   *
   * 힌트:
   *   while (Rio_readlineb(client_rio, buf, MAXLINE) > 0) {
   *       if (!strcmp(buf, "\r\n"))
   *           break;
   *       ...
   *   }
   */

  /*
   * TODO 4. Host 헤더는 따로 저장한다.
   *
   * 클라이언트가 Host 헤더를 보냈으면 그 값을 사용할 수 있다.
   *
   * 힌트:
   *   if (!strncasecmp(buf, "Host:", 5)) {
   *       strcpy(host_hdr, buf);
   *   }
   *
   * strncasecmp는 앞의 n글자만 대소문자 무시하고 비교한다.
   */

  /*
   * TODO 5. Connection, Proxy-Connection, User-Agent 헤더는 무시한다.
   *
   * 이유:
   *   프록시는 이 헤더들을 직접 정해진 값으로 다시 보낼 것이기 때문이다.
   *
   * 무시할 헤더:
   *   Connection:
   *   Proxy-Connection:
   *   User-Agent:
   */

  /*
   * TODO 6. 그 외 헤더는 other_hdr 뒤에 이어 붙인다.
   *
   * 힌트:
   *   strcat(other_hdr, buf);
   *
   * 예:
   *   Accept: */*\r\n
   * 같은 헤더는 유지해도 된다.
   */

  /*
   * TODO 7. 클라이언트가 Host 헤더를 보내지 않았다면 직접 만든다.
   *
   * 포트가 80이면:
   *   Host: example.com\r\n
   *
   * 포트가 80이 아니면:
   *   Host: localhost:8000\r\n
   *
   * 처음 Basic에서는 항상 "Host: host:port\r\n"로 만들어도 동작은 할 수 있다.
   */

  /*
   * TODO 8. 최종 http_header를 조립한다.
   *
   * 반드시 포함할 것:
   *   request_hdr
   *   host_hdr
   *   user_agent_hdr
   *   "Connection: close\r\n"
   *   "Proxy-Connection: close\r\n"
   *   other_hdr
   *   "\r\n"
   *
   * 힌트:
   *   sprintf(http_header, "%s%s%s%s%s%s\r\n",
   *           request_hdr,
   *           host_hdr,
   *           user_agent_hdr,
   *           "Connection: close\r\n",
   *           "Proxy-Connection: close\r\n",
   *           other_hdr);
   */
}
