#ifndef SOCKETSERVER_H
#define SOCKETSERVER_H

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <netinet/in.h>

#define LISTEN_PORT 3308

void start_tcp_server();

#endif // SOCKETSERVER_H
