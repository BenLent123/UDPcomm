#include <stdio.h>
#include "yggdrasil.h"
#include "socket.h"
#include "UDP.h"
#include "chat.h"
#include "qr.h"

int main(){
    char ipv6[INET6_ADDRSTRLEN];
    int socket_family = AF_INET6; // AF_INET6 for IPV6
    int port;
    int sockfd;
    struct sockaddr_in6 *peeraddr;
    char peer_ipv6[INET6_ADDRSTRLEN];
    int peer_port;

    //check if ipv6 mesh "yggdrasil" is on
    if(check_status_yggdrasil()==INACTIVE){
        printf("yggdrasil is off\n");
        start_yggdrasil();
        printf("starting yggdrasil\n");
    }else if(check_status_yggdrasil()==ACTIVE){
        printf("yggdrasil is already running\n");
    }else{
        printf("yggdrasil status check failed\n");
        return YGGDRASIL_INIT_ERROR;
    }

    printf("yggdrasil -- status check -- passed\n"); //sanity print

    // get own yggdrasil IPV6 
    if (get_self_ipv6(ipv6, sizeof(ipv6)) == 0) {
    } else {
        printf("yggdrasil -- failed to get self IPv6 address\n");
        return YGGDRASIL_IP_ERROR;
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
 

    printf("your ID: [%s]:%d\n", ipv6, port); // sanity print
    create_qr(ipv6, port); // generates a QR code image file "myid.png" with the ID
    printf("QR code saved to myid.png\n");
    printf("----PEER SECTION----\n");
    //enter peer IPV6
    peeraddr = malloc(sizeof(struct sockaddr_in6));
    if (peeraddr == NULL) {
        perror("malloc");
        exit(EXIT_FAILURE);
    }
    do {
        printf("Enter peer IPv6 address: ");
        if (scanf("%45s", peer_ipv6) != 1) {
            while (getchar() != '\n');
            continue;
        }
        if (inet_pton(AF_INET6, peer_ipv6, &peeraddr->sin6_addr) == 1) {
            break;
        } else if (strcmp(peer_ipv6, "exit") == 0) {
            free(peeraddr);
            return 0; // Exit program
        } else {
            printf("Invalid IPv6 address. Try again.\n");
        }
    } while (1);

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

    sockfd = create_and_bind_socket(ipv6,port); // creates a UDP socket with IP and Port

    udp_holepunch_and_chat(sockfd, peeraddr); // starts the chat
    
    return 0;
    
}