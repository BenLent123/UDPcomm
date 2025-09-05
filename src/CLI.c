#include <stdio.h>
#include "yggdrasil.h"
#include "socket.h"
#include "UDP.h"
#include "chat.h"
#include "qr.h"

void print_help(const char *prog) {
    printf("Usage:\n");
    printf("  %s <myport> <peer ipv6> <peerport> --> main usage\n", prog);
    printf(" %s -s --> gets own IPV6\n", prog);
    printf("  %s -l <peername> --> gets saved person from file\n", prog);
    printf("  %s -h | -help --> prints this\n", prog);
}

void print_title(void){
    printf("*****************************************\n");
    printf("\n");
    printf("----YGGDCOMM----\n");
    printf("\n");
    printf("!exit at any input prompt by typing 'exit'!\n");
    printf("\n");
    printf("*****************************************\n");
    printf("\n");
}

int main(int argc, char *argv[]) {
    char ipv6[INET6_ADDRSTRLEN];
    int socket_family = AF_INET6; // AF_INET6 for IPV6
    int my_port;
    int sockfd;
    struct sockaddr_in6 *peeraddr;
    char peer_ipv6[INET6_ADDRSTRLEN];
    int peer_port;
    
    if (argc == 1 || (argc == 2 && (strcmp(argv[1], "-h") == 0 || strcmp(argv[1], "-help") == 0))) {
        print_help(argv[0]);
        return 0;
    }

    if (argc == 2 && strcmp(argv[1],"-s")==0){
    //check if ipv6 mesh "yggdrasil" is on
        if(yggdrasil_sys_check()!=0){
            return -1;
        }
        // get own yggdrasil IPV6 
        if (get_self_ipv6(ipv6, sizeof(ipv6)) == 0) {
        } else {
            printf("yggdrasil -- failed to get self IPv6 address\n");
            return YGGDRASIL_IP_ERROR;
        }
        return SUCCESS;
    }

    if (argc == 3 && strcmp(argv[1], "-l") == 0) {
        // Stub: lookup peer by name in yggdrasil.conf
        printf("Looking up peer named '%s' in yggdrasil.conf (not implemented)\n", argv[2]);
        return 0;
    }

    if (argc == 4) {
        my_port = atoi(argv[1]);
        peer_port = atoi(argv[3]);
        strncpy(peer_ipv6, argv[2], INET6_ADDRSTRLEN - 1);
        peer_ipv6[INET6_ADDRSTRLEN - 1] = '\0';
        if(yggdrasil_sys_check()!=0){
            return -1;
        }

        if (my_port < 1 || my_port > 65535) {
            printf("Invalid port (my_port): %d\n", my_port);
            return 1;
        }
        if (inet_pton(AF_INET6, peer_ipv6, &peeraddr) != 1) {
            printf("Invalid peer IPv6 address: %s\n", peer_ipv6);
            return 1;
        }
        if (peer_port < 1 || peer_port > 65535) {
            printf("Invalid port (peerPort): %d\n", peer_port);
            return 1;
        }
        printf("--all as expected --\n");
        
        create_qr(ipv6, my_port); // generates a QR code image file "myid.png" with the ID
        printf("QR code saved to myid.png\n");

        peeraddr->sin6_family = socket_family;
        peeraddr->sin6_port = htons(peer_port);

        sockfd = create_and_bind_socket(ipv6,my_port); // creates a UDP socket with your IP and Port

        udp_holepunch_and_chat(sockfd, peeraddr); // starts the chat



        return 0;
    }

    print_help(argv[0]);
    return -1;
}