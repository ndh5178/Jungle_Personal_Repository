#include "../csapp.h"
#include <stdio.h>
#include <string.h>
/*
 * Echo 서버 연습.
 *
 * 이 서버는 일부러 "반복형 서버"로 만든다.
 * - 클라이언트 1명을 받는다.
 * - 그 클라이언트가 보낸 줄을 그대로 다시 보내준다.
 * - 클라이언트가 연결을 끊으면 connfd를 닫는다.
 * - 다시 다음 클라이언트를 기다린다.
 *
 * Tiny의 main 반복문과 바깥 구조는 거의 같다.
 * 다만 Tiny의 doit()보다 처리 내용이 훨씬 단순하다.
 */

typedef struct {
    int choseong;
    int jungseong;
    int jongseong;
} HangulState;


void echo(int connfd);
void eng_to_hangul_simple(const char *in, char *out);
int main(int argc, char **argv)
{
    int listenfd;
    int connfd;
    socklen_t clientlen;
    struct sockaddr_storage clientaddr;
    char client_hostname[MAXLINE];
    char client_port[MAXLINE];

    /*
     * TODO 1. 명령행 인자 개수를 확인한다.
     *
     * 기대하는 실행 명령:
     *   ./echoserver <port>
     *
     * 기억할 것:
     *   argv[0] = "./echoserver"
     *   argv[1] = 포트 문자열, 예: "8000"
     *
     * argc가 2가 아니면:
     *   - stderr에 사용법을 출력한다.
     *   - exit으로 프로그램을 종료한다.
     */
    if(argc!=2){
        fprintf(stderr, "usage: %s <port>\n", argv[0]);
        exit(1);
    }

    /*
     * TODO 2. 서버 문 역할을 하는 listening socket을 연다.
     *
     * Open_listenfd(argv[1])를 사용한다.
     *
     * 결과:
     *   listenfd는 서버가 새 클라이언트를 기다리는 소켓 번호다.
     *
     * 머릿속 그림:
     *   fd 3 -> TCP listen socket, local port argv[1], state LISTEN
     */
    listenfd=Open_listenfd(argv[1]);

    /*
     * TODO 3. 무한 반복 서버 루프에 들어간다.
     *
     * 서버는 계속 켜져 있으면서 클라이언트를 받아야 한다.
     *
     * 반복문 안에서 할 일:
     *   1. clientlen = sizeof(clientaddr)로 초기화한다.
     *   2. Accept(listenfd, (SA *)&clientaddr, &clientlen)를 호출한다.
     *      - 클라이언트가 접속할 때까지 여기서 기다린다.
     *      - 접속이 오면 실제 대화용 fd인 connfd를 리턴한다.
     *      - clientaddr 안에는 클라이언트 주소 정보가 채워진다.
     *   3. Getnameinfo로 clientaddr을 출력 가능한 문자열로 바꾼다.
     *   4. 디버깅용으로 "Connected to (...)"를 출력한다.
     *   5. echo(connfd)를 호출해서 실제 대화를 처리한다.
     *   6. Close(connfd)로 이 클라이언트와의 연결을 닫는다.
     *
     * 중요한 구분:
     *   listenfd = 새 클라이언트가 들어오는 서버 문
     *   connfd   = 이미 들어온 클라이언트 1명과 대화하는 통로
     */
    while (1)
    {
        clientlen=sizeof(clientaddr);
        connfd=Accept(listenfd,(SA*)&clientaddr,&clientlen);
        Getnameinfo((SA *)&clientaddr, clientlen, &client_hostname[0], MAXLINE, &client_port[0], MAXLINE,0);
        printf("Accepted connection from (%s, %s)\n", client_hostname, client_port);
        echo(connfd);
        printf("Closeed connection from (%s, %s)\n", client_hostname, client_port);
        Close(connfd);

    }
    

    return 0;
}

void echo(int connfd)
{
    size_t n;
    char buf[MAXLINE],result[MAXLINE];
    rio_t rio;

    /*
     * TODO 4. RIO 버퍼를 초기화한다.
     *
     * Rio_readinitb(&rio, connfd)를 사용한다.
     *
     * 여기서는 아직 데이터를 읽지 않는다.
     * rio라는 읽기 상태 구조체를 connfd와 연결해두는 단계다.
     */
    Rio_readinitb(&rio,connfd);
    /*
     * TODO 5. 클라이언트가 연결을 끊을 때까지 한 줄씩 읽는다.
     *
     * while 반복문을 사용한다.
     *
     *   while ((n = Rio_readlineb(&rio, buf, MAXLINE)) != 0) {
     *       ...
     *   }
     *
     * 의미:
     *   - Rio_readlineb는 한 줄이 도착할 때까지 기다릴 수 있다.
     *   - 도착한 한 줄을 buf에 저장한다.
     *   - 읽은 바이트 수를 n으로 리턴한다.
     *   - 클라이언트가 연결을 끊으면 0을 리턴한다.
     */


    /*
     * TODO 6. 반복문 안에서 몇 바이트를 받았는지 출력한다.
     *
     * 예시:
     *   printf("server received %d bytes\n", (int)n);
     */

    /*
     * TODO 7. 반복문 안에서 받은 내용을 그대로 클라이언트에게 다시 보낸다.
     *
     * Rio_writen(connfd, buf, n)을 사용한다.
     *
     * 이 동작 때문에 Echo 서버라고 부른다.
     *
     *   클라이언트가 줄을 보냄 -> 서버가 같은 줄을 다시 보냄
     */
    while ((n=Rio_readlineb(&rio,buf,MAXLINE))!=0){
        printf("server received %d bytes\n",(int)n);
        eng_to_hangul_simple(buf,result);
        Rio_writen(connfd,result,strlen(result));
    }
}


int key_to_choseong(char c)
{
    switch (c) {
    case 'r': return 0;   // ㄱ
    case 'R': return 1;   // ㄲ
    case 's': return 2;   // ㄴ
    case 'e': return 3;   // ㄷ
    case 'E': return 4;   // ㄸ
    case 'f': return 5;   // ㄹ
    case 'a': return 6;   // ㅁ
    case 'q': return 7;   // ㅂ
    case 'Q': return 8;   // ㅃ
    case 't': return 9;   // ㅅ
    case 'T': return 10;  // ㅆ
    case 'd': return 11;  // ㅇ
    case 'w': return 12;  // ㅈ
    case 'W': return 13;  // ㅉ
    case 'c': return 14;  // ㅊ
    case 'z': return 15;  // ㅋ
    case 'x': return 16;  // ㅌ
    case 'v': return 17;  // ㅍ
    case 'g': return 18;  // ㅎ
    default: return -1;
    }
}

int key_to_jungseong(char c)
{
    switch (c) {
    case 'k': return 0;   // ㅏ
    case 'o': return 1;   // ㅐ
    case 'i': return 2;   // ㅑ
    case 'O': return 3;   // ㅒ
    case 'j': return 4;   // ㅓ
    case 'p': return 5;   // ㅔ
    case 'u': return 6;   // ㅕ
    case 'P': return 7;   // ㅖ
    case 'h': return 8;   // ㅗ
    case 'y': return 12;  // ㅛ
    case 'n': return 13;  // ㅜ
    case 'b': return 17;  // ㅠ
    case 'm': return 18;  // ㅡ
    case 'l': return 20;  // ㅣ
    default: return -1;
    }
}

int choseong_to_jongseong(int choseong)
{
    switch (choseong) {
    case 0: return 1;    // ㄱ
    case 1: return 2;    // ㄲ
    case 2: return 4;    // ㄴ
    case 3: return 7;    // ㄷ
    case 5: return 8;    // ㄹ
    case 6: return 16;   // ㅁ
    case 7: return 17;   // ㅂ
    case 9: return 19;   // ㅅ
    case 10: return 20;  // ㅆ
    case 11: return 21;  // ㅇ
    case 12: return 22;  // ㅈ
    case 14: return 23;  // ㅊ
    case 15: return 24;  // ㅋ
    case 16: return 25;  // ㅌ
    case 17: return 26;  // ㅍ
    case 18: return 27;  // ㅎ
    default: return -1;
    }
}

void append_utf8(char *out, int *out_len, int codepoint)
{
    if (codepoint <= 0x7F) {
        out[(*out_len)++] = codepoint;
    } else if (codepoint <= 0x7FF) {
        out[(*out_len)++] = 0xC0 | ((codepoint >> 6) & 0x1F);
        out[(*out_len)++] = 0x80 | (codepoint & 0x3F);
    } else {
        out[(*out_len)++] = 0xE0 | ((codepoint >> 12) & 0x0F);
        out[(*out_len)++] = 0x80 | ((codepoint >> 6) & 0x3F);
        out[(*out_len)++] = 0x80 | (codepoint & 0x3F);
    }
}

void append_hangul_syllable(char *out, int *out_len,
                            int cho, int jung, int jong)
{
    int codepoint = 0xAC00 + (cho * 21 + jung) * 28 + jong;
    append_utf8(out, out_len, codepoint);
}

void eng_to_hangul_simple(const char *in, char *out)
{
    int i = 0;
    int out_len = 0;

    while (in[i] != '\0') {
        int cho = key_to_choseong(in[i]);
        int jung = key_to_jungseong(in[i + 1]);

        if (cho != -1 && jung != -1) {
            int jong = 0;
            int next_cho = key_to_choseong(in[i + 2]);
            int next_jung = key_to_jungseong(in[i + 3]);

            if (next_cho != -1 && next_jung == -1) {
                int possible_jong = choseong_to_jongseong(next_cho);
                if (possible_jong != -1) {
                    jong = possible_jong;
                    i += 3;
                } else {
                    i += 2;
                }
            } else {
                i += 2;
            }

            append_hangul_syllable(out, &out_len, cho, jung, jong);
        } else {
            out[out_len++] = in[i];
            i++;
        }
    }

    out[out_len] = '\0';
}
