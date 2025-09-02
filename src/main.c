#include <stdio.h>
#include "yggdrasil.h"
#include "socket.h"
#include "UDP.h"
#include "chat.h"

int main(){
    char ipv6[INET6_ADDRSTRLEN];
    int socket_family = AF_INET6; // AF_INET or AF_INET6
    int socket_type = SOCK_DGRAM;  // SOCK_STREAM or SOCK_DGRAM
    int port;
    int sockfd;
    struct sockaddr_in6 *peeraddr;
    char peer_ipv6[INET6_ADDRSTRLEN];
    int valid_ipv6 = 0;
    int peer_port;
    //check if ipv6 mesh "yggdrasil" is on
    if(check_status_yggdrasil()!=0){
        printf("yggdrasil is off\n");
        start_yggdrasil();
    }else{
        printf("yggdrasil is already running\n");
    }

    printf("yggdrasil -- status check -- passed\n"); //sanity print

    // get own yggdrasil IPV6 
    if (get_self_ipv6(ipv6, sizeof(ipv6)) == 0) {
        printf("yggdrasil -- self IPv6 address: %s\n", ipv6);
    } else {
        printf("yggdrasil -- failed to get self IPv6 address\n");
    }

    //decide on a port
    do {
        printf("Enter UDP port (3000-9999): ");
        if (scanf("%d", &port) != 1) {
            // Invalid input, clear stdin
            while (getchar() != '\n');
            port = 0;
        }
    } while (port < 3000 || port > 9999);

    printf("your ID: %d+%s\n", port, ipv6); // sanity print
    
    //enter peer IPV6
    peeraddr = malloc(sizeof(struct sockaddr_in6));
    if (peeraddr == NULL) {
        perror("malloc");
        exit(EXIT_FAILURE);
    }
    printf("Enter peer IPv6 address: ");
    scanf("%45s", peer_ipv6);
    inet_pton(AF_INET6, peer_ipv6, &peeraddr->sin6_addr);

    //enter peer port
    do {
        printf("Enter peer UDP port (3000-9999): ");
        if (scanf("%d", &peer_port) != 1) {
            while (getchar() != '\n');
            peer_port = 0;
        }
    } while (peer_port < 3000 || peer_port > 9999);

    peeraddr->sin6_family = socket_family;
    peeraddr->sin6_port = htons(peer_port);

    sockfd = create_and_bind_socket(socket_family,socket_type,ipv6,port); // creates a UDP socket with IP and Port

    udp_holepunch_and_chat(sockfd, peeraddr);
    
    return 0;
    
}