#include "UDP.h"
#include "chat.h"

int main() {
    int sockfd;
    struct sockaddr_in6 bindaddr, peeraddr;
    char peer_ip[128];
    int peer_port;
    int bind_port;

    printf("Enter local port to bind to: ");
    scanf("%d", &bind_port);

    // Create IPv6 UDP socket
    sockfd = socket(AF_INET6, SOCK_DGRAM, 0);
    if (sockfd < 0) { perror("socket"); return 1; }

    memset(&bindaddr, 0, sizeof(bindaddr));
    bindaddr.sin6_family = AF_INET6;
    bindaddr.sin6_addr = in6addr_any;
    bindaddr.sin6_port = htons(bind_port);
    if (bind(sockfd, (struct sockaddr*)&bindaddr, sizeof(bindaddr)) < 0) {
        perror("bind"); close(sockfd); return 1;
    }
    printf("Bound to local port %d (IPv6)\n", bind_port);

    printf("Enter peer's IPv6 address: ");
    scanf("%127s", peer_ip);
    printf("Enter peer's port: ");
    scanf("%d", &peer_port);

    memset(&peeraddr, 0, sizeof(peeraddr));
    peeraddr.sin6_family = AF_INET6;
    peeraddr.sin6_port = htons(peer_port);
    if (inet_pton(AF_INET6, peer_ip, &peeraddr.sin6_addr) != 1) {
        printf("Invalid peer IPv6 address.\n");
        close(sockfd);
        return 1;
    }

    // UDP hole punching: send initial packet to peer
    char punch_msg[] = "UDP Connection Packet";
    if(sendto(sockfd, punch_msg, strlen(punch_msg), 0, (struct sockaddr*)&peeraddr, sizeof(peeraddr)) < 0) {
        perror("sendto");
        close(sockfd);
        return 1;
    }

    chat_loop(sockfd, &peeraddr);

    close(sockfd);
    return 0;
}
