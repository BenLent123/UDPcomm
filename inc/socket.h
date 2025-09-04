#ifndef SOCKET_H
#define SOCKET_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netdb.h>
#include "defines.h"

// Creates and binds a socket
// ip: IP address as string (NULL for INADDR_ANY/in6addr_any)
// port: port number
// Returns socket fd on success, -1 on error
int create_and_bind_socket(const char *ip, int port);

#endif // SOCKET_H
