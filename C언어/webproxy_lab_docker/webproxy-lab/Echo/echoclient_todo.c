#include "csapp.h"

/*
 * Optional echo client practice.
 *
 * This client:
 * - Connects to an echo server.
 * - Reads one line from stdin.
 * - Sends that line to the server.
 * - Reads the echoed line from the server.
 * - Prints it to stdout.
 */

int main(int argc, char **argv)
{
    int clientfd;
    char *host;
    char *port;
    char buf[MAXLINE];
    rio_t rio;

    /*
     * TODO 1. Check command-line arguments.
     *
     * Expected command:
     *   ./echoclient <host> <port>
     *
     * Example:
     *   ./echoclient localhost 8000
     *
     * If argc is not 3:
     *   - print usage to stderr
     *   - exit
     */

    /*
     * TODO 2. Save argv[1] and argv[2].
     *
     * host = argv[1]
     * port = argv[2]
     */

    /*
     * TODO 3. Connect to the server.
     *
     * Use Open_clientfd(host, port).
     *
     * Result:
     *   clientfd is a connected socket descriptor.
     */

    /*
     * TODO 4. Initialize an RIO buffer for clientfd.
     *
     * Use Rio_readinitb(&rio, clientfd).
     */

    /*
     * TODO 5. Loop over stdin.
     *
     * Use:
     *   while (Fgets(buf, MAXLINE, stdin) != NULL) {
     *       ...
     *   }
     *
     * Each iteration:
     *   1. Write buf to the server with Rio_writen(clientfd, buf, strlen(buf)).
     *   2. Read one echoed line from the server with Rio_readlineb(&rio, buf, MAXLINE).
     *   3. Print buf to stdout with Fputs(buf, stdout).
     */

    /*
     * TODO 6. Close clientfd before exiting.
     */

    return 0;
}

