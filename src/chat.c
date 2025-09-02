#include "chat.h"

void chat_loop(int sockfd, struct sockaddr_in6 *peeraddr) {
    fd_set readfds;
    char sendbuf[512], recvbuf[512];
    printf("Chat started. Type messages and press Enter.\n");
    printf("Type 'exit' to quit.\n");
    while (1) {
        FD_ZERO(&readfds);
        FD_SET(0, &readfds); // stdin
        FD_SET(sockfd, &readfds);
        int maxfd = sockfd;
        int activity = select(maxfd+1, &readfds, NULL, NULL, NULL);
        if (activity < 0) continue;

        // Incoming message
        if (FD_ISSET(sockfd, &readfds)) {
            struct sockaddr_in6 from;
            socklen_t fromlen = sizeof(from);
            int rlen = recvfrom(sockfd, recvbuf, sizeof(recvbuf)-1, 0, (struct sockaddr*)&from, &fromlen);
            if (rlen > 0) {
                recvbuf[rlen] = '\0';
                printf("Peer: %s\n", recvbuf);
            }
        }

        // User input
        if (FD_ISSET(0, &readfds)) {
            if (fgets(sendbuf, sizeof(sendbuf), stdin)) {
                size_t slen = strlen(sendbuf);
                if (slen > 0 && sendbuf[slen-1] == '\n') sendbuf[slen-1] = '\0';
                if (strcmp(sendbuf, "exit") == 0) break;
                sendto(sockfd, sendbuf, strlen(sendbuf), 0, (struct sockaddr*)peeraddr, sizeof(*peeraddr));
            }
        }
    }
}
