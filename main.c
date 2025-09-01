#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netdb.h>

#include <sys/select.h>
#include <time.h>

#define STUN_SERVER "stun.l.google.com"
#define STUN_PORT 19302

// STUN message types
#define STUN_BINDING_REQUEST 0x0001
#define STUN_BINDING_RESPONSE 0x0101

// STUN attribute types
#define STUN_ATTR_MAPPED_ADDRESS 0x0001

// STUN header size
#define STUN_HEADER_SIZE 20

// Helper to build a simple STUN binding request
void build_stun_request(unsigned char *buf, size_t len) {
	memset(buf, 0, len);
	buf[0] = 0x00; buf[1] = 0x01; // Binding Request
	buf[2] = 0x00; buf[3] = 0x00; // Message Length
	// Transaction ID (random)
	for (int i = 4; i < 20; ++i) buf[i] = rand() % 256;
}

// Parse STUN response for MAPPED-ADDRESS
int parse_stun_response(unsigned char *buf, int len, char *ip, int *port) {
	int i = STUN_HEADER_SIZE;
	while (i < len) {
		unsigned short attr_type = (buf[i] << 8) | buf[i+1];
		unsigned short attr_len = (buf[i+2] << 8) | buf[i+3];
		if (attr_type == STUN_ATTR_MAPPED_ADDRESS && attr_len >= 8) {
			int port_offset = i + 6;
			int ip_offset = i + 8;
			*port = (buf[port_offset] << 8) | buf[port_offset+1];
			sprintf(ip, "%d.%d.%d.%d", buf[ip_offset], buf[ip_offset+1], buf[ip_offset+2], buf[ip_offset+3]);
			return 0;
		}
		i += 4 + attr_len;
	}
	return -1;
}

int main() {
	srand(time(NULL));
	int sockfd;
	struct sockaddr_in servaddr, peeraddr, bindaddr;
	unsigned char stun_req[STUN_HEADER_SIZE];
	unsigned char stun_resp[512];
	char public_ip[32];
	int public_port;

	// Ask for local testing
	char localtest[8];
	printf("Local testing? (y/n): ");
	scanf("%7s", localtest);

	int use_local = (localtest[0] == 'y' || localtest[0] == 'Y');

	int bind_port = 0;
	if (use_local) {
		printf("Enter local port to bind to: ");
		scanf("%d", &bind_port);
	}

	// Create UDP socket
	sockfd = socket(AF_INET, SOCK_DGRAM, 0);
	if (sockfd < 0) { perror("socket"); return 1; }

	if (use_local) {
		memset(&bindaddr, 0, sizeof(bindaddr));
		bindaddr.sin_family = AF_INET;
		bindaddr.sin_addr.s_addr = htonl(INADDR_ANY);
		bindaddr.sin_port = htons(bind_port);
		if (bind(sockfd, (struct sockaddr*)&bindaddr, sizeof(bindaddr)) < 0) {
			perror("bind"); close(sockfd); return 1;
		}
		printf("Bound to local port %d\n", bind_port);
		strcpy(public_ip, "127.0.0.1");
		public_port = bind_port;
		printf("Your local IP: %s\n", public_ip);
		printf("Your local port: %d\n", public_port);
	} else {
		// Resolve STUN server
		struct hostent *he = gethostbyname(STUN_SERVER);
		if (!he) { perror("gethostbyname"); return 1; }

		memset(&servaddr, 0, sizeof(servaddr));
		servaddr.sin_family = AF_INET;
		servaddr.sin_port = htons(STUN_PORT);
		memcpy(&servaddr.sin_addr, he->h_addr_list[0], he->h_length);

		// Build and send STUN request
		build_stun_request(stun_req, sizeof(stun_req));
		if (sendto(sockfd, stun_req, sizeof(stun_req), 0, (struct sockaddr*)&servaddr, sizeof(servaddr)) < 0) {
			perror("sendto"); close(sockfd); return 1;
		}

		// Receive STUN response
		socklen_t servlen = sizeof(servaddr);
		int n = recvfrom(sockfd, stun_resp, sizeof(stun_resp), 0, (struct sockaddr*)&servaddr, &servlen);
		if (n < 0) { perror("recvfrom"); close(sockfd); return 1; }

		// Parse public IP and port
		if (parse_stun_response(stun_resp, n, public_ip, &public_port) == 0) {
			printf("Your public IP: %s\n", public_ip);
			printf("Your public port: %d\n", public_port);
		} else {
			printf("Failed to parse STUN response.\n");
			close(sockfd);
			return 1;
		}
	}

	// Prompt for peer info
	char peer_ip[32];
	int peer_port;
	if (use_local) {
		printf("Enter peer's local port: ");
		scanf("%d", &peer_port);
		strcpy(peer_ip, "127.0.0.1");
	} else {
		printf("Enter peer's public IP: ");
		scanf("%31s", peer_ip);
		printf("Enter peer's public port: ");
		scanf("%d", &peer_port);
	}

	memset(&peeraddr, 0, sizeof(peeraddr));
	peeraddr.sin_family = AF_INET;
	peeraddr.sin_port = htons(peer_port);
	if (inet_pton(AF_INET, peer_ip, &peeraddr.sin_addr) != 1) {
		printf("Invalid peer IP.\n");
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

	printf("Chat started. Type messages and press Enter.\n");
	printf("Type 'exit' to quit.\n");

	fd_set readfds;
	char sendbuf[512], recvbuf[512];
	while (1) {
		FD_ZERO(&readfds);
		FD_SET(0, &readfds); // stdin
		FD_SET(sockfd, &readfds);
		int maxfd = sockfd;
		int activity = select(maxfd+1, &readfds, NULL, NULL, NULL);
		if (activity < 0) continue;

		// Incoming message
		if (FD_ISSET(sockfd, &readfds)) {
			struct sockaddr_in from;
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
				sendto(sockfd, sendbuf, strlen(sendbuf), 0, (struct sockaddr*)&peeraddr, sizeof(peeraddr));
			}
		}
	}

	close(sockfd);
	return 0;
}
