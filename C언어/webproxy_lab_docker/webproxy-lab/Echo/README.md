# Echo Server TODO Practice

This folder is a small CS:APP Chapter 11 practice scaffold.

Goal:

1. Build an iterative echo server.
2. Understand `Open_listenfd`, `Accept`, `Getnameinfo`, `Rio_readinitb`,
   `Rio_readlineb`, and `Rio_writen`.
3. See the simpler pattern that later becomes Tiny:

```text
listen -> accept -> read request line(s) -> write response -> close
```

Recommended order:

1. Open `echoserver_todo.c`.
2. Fill the TODOs in `main`.
3. Fill the TODOs in `echo`.
4. Build with `make server`.
5. Run the server: `./echoserver 8000`
6. In another terminal, connect with netcat or telnet:

```bash
nc localhost 8000
```

Then type lines. The server should send each line back exactly as received.

Optional:

1. Fill `echoclient_todo.c`.
2. Build with `make client`.
3. Run: `./echoclient localhost 8000`

