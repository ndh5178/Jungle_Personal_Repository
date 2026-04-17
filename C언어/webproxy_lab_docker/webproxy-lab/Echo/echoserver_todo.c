#include "csapp.h"

/*
 * Echo server practice.
 *
 * This server is intentionally iterative:
 * - It accepts one client.
 * - It echoes that client's lines until the client closes the connection.
 * - Then it closes connfd and waits for the next client.
 *
 * This is the same outer shape as Tiny's main loop, but the request handler
 * is much simpler than Tiny's doit().
 */

void echo(int connfd);

int main(int argc, char **argv)
{
    int listenfd;
    int connfd;
    socklen_t clientlen;
    struct sockaddr_storage clientaddr;
    char client_hostname[MAXLINE];
    char client_port[MAXLINE];

    /*
     * TODO 1. Check command-line arguments.
     *
     * Expected command:
     *   ./echoserver <port>
     *
     * Remember:
     *   argv[0] = "./echoserver"
     *   argv[1] = port string, for example "8000"
     *
     * If argc is not 2:
     *   - print usage to stderr
     *   - exit
     */

    /*
     * TODO 2. Open a listening socket.
     *
     * Use Open_listenfd(argv[1]).
     *
     * Result:
     *   listenfd is a descriptor for the server's listening socket.
     *   Example mental model:
     *     fd 3 -> TCP listen socket, local port argv[1], state LISTEN
     */

    /*
     * TODO 3. Enter the infinite server loop.
     *
     * The server should keep accepting clients forever.
     *
     * Inside the loop:
     *   1. Set clientlen to sizeof(clientaddr).
     *   2. Call Accept(listenfd, (SA *)&clientaddr, &clientlen).
     *      - Accept blocks until a client connects.
     *      - It returns a connected descriptor, connfd.
     *      - It fills clientaddr with the client's address.
     *   3. Call Getnameinfo to convert clientaddr into printable strings.
     *   4. Print "Connected to (...)" for debugging.
     *   5. Call echo(connfd).
     *   6. Close(connfd).
     *
     * Important distinction:
     *   listenfd = the door for new clients
     *   connfd   = the conversation channel for one accepted client
     */

    return 0;
}

void echo(int connfd)
{
    size_t n;
    char buf[MAXLINE];
    rio_t rio;

    /*
     * TODO 4. Initialize the RIO buffer.
     *
     * Use Rio_readinitb(&rio, connfd).
     *
     * This does not read data yet.
     * It only connects the rio state object to connfd.
     */

    /*
     * TODO 5. Read lines until the client closes the connection.
     *
     * Use a while loop:
     *
     *   while ((n = Rio_readlineb(&rio, buf, MAXLINE)) != 0) {
     *       ...
     *   }
     *
     * Meaning:
     *   - Rio_readlineb blocks until a line arrives, EOF, or error.
     *   - It stores one line in buf.
     *   - It returns the number of bytes read.
     *   - It returns 0 when the client closes its end of the connection.
     */

    /*
     * TODO 6. Inside the loop, print how many bytes arrived.
     *
     * Example:
     *   printf("server received %d bytes\n", (int)n);
     */

    /*
     * TODO 7. Inside the loop, write the same bytes back to the client.
     *
     * Use Rio_writen(connfd, buf, n).
     *
     * This is what makes it an echo server:
     *   client sends line -> server sends the same line back
     */
}

