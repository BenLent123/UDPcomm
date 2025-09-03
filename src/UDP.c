#include "UDP.h"
#include "chat.h"

// Performs UDP hole punching and starts chat loop
int udp_holepunch_and_chat(int sockfd, struct sockaddr_in6 *peeraddr) {
    char punch_msg[] = "-- UDP Connection Packet --";

    if(sendto(sockfd, punch_msg, strlen(punch_msg), 0, (struct sockaddr*)peeraddr, sizeof(*peeraddr)) < 0) {
        perror("sendto");
        return -1;
    }

    chat_loop(sockfd, peeraddr);
    
    return 0;
}

