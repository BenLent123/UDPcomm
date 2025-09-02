#ifndef SOCKET_H
#define SOCKET_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netdb.h>

// Creates and binds a socket
// family: AF_INET or AF_INET6
// type: SOCK_STREAM (TCP) or SOCK_DGRAM (UDP)
// ip: IP address as string (NULL for INADDR_ANY/in6addr_any)
// port: port number
// Returns socket fd on success, -1 on error
int create_and_bind_socket(int family, int type, const char *ip, int port);

#endif // SOCKET_H
