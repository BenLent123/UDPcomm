#include "socket.h"

int create_and_bind_socket(int family, int type, const char *ip, int port) {
    int sockfd;
    if (family == AF_INET){
        struct sockaddr_in addr;
        sockfd = socket(AF_INET, type, 0);

        if (sockfd < 0){
            perror("socket");
            return INVALID_SOCKFD;
        }

        memset(&addr, 0, sizeof(addr));
        addr.sin_family = AF_INET;
        addr.sin_port = htons(port);

        if (ip == NULL) {
            addr.sin_addr.s_addr = htonl(INADDR_ANY);
        } else {
            if (inet_pton(AF_INET, ip, &addr.sin_addr) != 1) {
                printf("Invalid IPv4 address.\n");
                close(sockfd);
                return INVALID_IPV4;
            }
        }
        if (bind(sockfd, (struct sockaddr*)&addr, sizeof(addr)) < 0) {
            perror("bind");
            close(sockfd);
            return BINDING_ERROR;
        }
    } else if (family == AF_INET6) {
        struct sockaddr_in6 addr6;
        sockfd = socket(AF_INET6, type, 0);

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
    } else {
        printf("Unsupported address family.\n");
        return DEFAULT_ERROR;
    }
    return sockfd;
}
