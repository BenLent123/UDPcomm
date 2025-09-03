#ifndef UDP_H
#define UDP_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netdb.h>
#include <sys/select.h>
#include <time.h>

#define SUCCESS 0
#define SENDTO_FAILED -1

// Performs UDP hole punching and starts chat loop
int udp_holepunch_and_chat(int sockfd, struct sockaddr_in6 *peeraddr);

#endif // UDP_H
