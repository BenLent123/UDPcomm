#include "socket.h"

int create_and_bind_socket(const char *ip, int port) {
    struct sockaddr_in6 addr6;
    int family = AF_INET6;
    int type = SOCK_DGRAM;
    int sockfd = socket(family, type, 0);
    if (sockfd < 0) {
        perror("socket");
        return INVALID_SOCKFD;
    }

    memset(&addr6, 0, sizeof(addr6));
    addr6.sin6_family = AF_INET6;
    addr6.sin6_port = htons(port);

    if (ip == NULL) {
        addr6.sin6_addr = in6addr_any;
    } else {
        if (inet_pton(AF_INET6, ip, &addr6.sin6_addr) != 1) {
            printf("Invalid IPv6 address.\n");
            close(sockfd);
            return INVALID_IPV6;
        }
        }
        if (bind(sockfd, (struct sockaddr*)&addr6, sizeof(addr6)) < 0) {
            perror("bind");
            close(sockfd);
            return BINDING_ERROR;
        }
 

    return sockfd;

}
