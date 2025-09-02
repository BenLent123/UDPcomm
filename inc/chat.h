#ifndef CHAT_H
#define CHAT_H

#include "UDP.h"

void chat_loop(int sockfd, struct sockaddr_in6 *peeraddr);

#endif // CHAT_H
