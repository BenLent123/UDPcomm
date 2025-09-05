#include <stdio.h>
#include "yggdrasil.h"
#include "socket.h"
#include "UDP.h"
#include "chat.h"
#include "qr.h"

int main(void){
    char ipv6[INET6_ADDRSTRLEN];
    int socket_family = AF_INET6; // AF_INET6 for IPV6
    int port;
    int sockfd;
    struct sockaddr_in6 *peeraddr;
    char peer_ipv6[INET6_ADDRSTRLEN];
    int peer_port;
    printf("*****************************************\n");
    printf("\n");
    printf("----YGGDCOMM----\n");
    printf("\n");
    printf("!exit at any input prompt by typing 'exit'!\n");
    printf("\n");
    printf("*****************************************\n");
    printf("\n");
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
    char port_input[16]; // buffer to handle user input
    do {
        printf("Enter UDP port (3000-9999) or 'exit': ");
        if (scanf("%15s", port_input) != 1) {
            while (getchar() != '\n');
            continue;
        }
        if (strcmp(port_input, "exit") == 0) {
            return USER_EXIT; // Exit program
        }
        port = atoi(port_input);
    } while (port < 3000 || port > 9999);
 

    printf("your ID: [%s]:%d\n", ipv6, port); // sanity print
    create_qr(ipv6, port); // generates a QR code image file "myid.png" with the ID
    printf("QR code saved to myid.png\n");
    printf("----PEER SECTION----\n");
    do {
    printf("choose how to enter peer info:\n");
    printf("1: manual input\n");
    printf("2: enter peerID\n");
    printf("Enter 1 or 2: ");
    if (scanf("%15s", port_input) != 1) {
        while (getchar() != '\n');
        continue;
    }
    if (strcmp(port_input, "exit") == 0) {
        return USER_EXIT;
    }
    } while (strcmp(port_input, "1") != 0 && strcmp(port_input, "2") != 0);

    if (strcmp(port_input, "1") == 0) {
    peeraddr = malloc(sizeof(struct sockaddr_in6));
    if (peeraddr == NULL) {
        perror("peeraddr malloc failed");
        exit(EXIT_FAILURE);
    }
    do {
        printf("Enter peer IPv6 address or 'exit': ");
        if (scanf("%45s", peer_ipv6) != 1) {
            while (getchar() != '\n');
            continue;
        }
        if (inet_pton(AF_INET6, peer_ipv6, &peeraddr->sin6_addr) == 1) {
            break;
        } else if (strcmp(peer_ipv6, "exit") == 0) {
            free(peeraddr);
            return USER_EXIT; // Exit program
        } else {
            printf("Invalid IPv6 address. Try again.\n");
        }
    } while (1);

    //enter peer port
    do {
        printf("Enter peer UDP port (3000-9999) or 'exit': ");
        if (scanf("%15s", port_input) != 1) {
            while (getchar() != '\n');
            continue;
        }
        if (strcmp(port_input, "exit") == 0) {
            free(peeraddr);
            return USER_EXIT; // Exit program
        }
        peer_port = atoi(port_input);
    } while (peer_port < 3000 || peer_port > 9999);

    } else if (strcmp(port_input, "2") == 0) {
        char peer_id_input[64];
        do {
            printf("Enter peer ID (format: [ipv6]:port) or 'exit': ");
            if (scanf("%63s", peer_id_input) != 1) {
                while (getchar() != '\n');
                continue;
            }
            if (strcmp(peer_id_input, "exit") == 0) {
                return USER_EXIT;
            }
            // Parse [ipv6]:port
            char *close_bracket = strchr(peer_id_input, ']');
            char *colon = strrchr(peer_id_input, ':');
            if (peer_id_input[0] == '[' && close_bracket && colon && colon > close_bracket) {
                size_t ipv6_len = close_bracket - peer_id_input - 1;
                if (ipv6_len < INET6_ADDRSTRLEN) {
                    strncpy(peer_ipv6, peer_id_input + 1, ipv6_len);
                    peer_ipv6[ipv6_len] = '\0';
                    peer_port = atoi(colon + 1);
                    if (peer_port >= 3000 && peer_port <= 9999) {
                        break;
                    }
                }
            }
            printf("Invalid peer ID format. Try again.\n");
        } while (1);
        peeraddr = malloc(sizeof(struct sockaddr_in6));
        if (peeraddr == NULL) {
            perror("peeraddr malloc failed");
            exit(EXIT_FAILURE);
        }
        if (inet_pton(AF_INET6, peer_ipv6, &peeraddr->sin6_addr) != 1) {
            printf("Invalid IPv6 address in peer ID.\n");
            free(peeraddr);
            return USER_EXIT;
        }

    }
    

    peeraddr->sin6_family = socket_family;
    peeraddr->sin6_port = htons(peer_port);

    sockfd = create_and_bind_socket(ipv6,port); // creates a UDP socket with IP and Port

    udp_holepunch_and_chat(sockfd, peeraddr); // starts the chat
    
    return 0;
    
}