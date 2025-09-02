#ifndef CHAT_H
#define CHAT_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netdb.h>
#include <sys/select.h>
#include <time.h>

void chat_loop(int sockfd, struct sockaddr_in6 *peeraddr);

#endif // CHAT_H
