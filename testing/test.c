#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>

int main(int argc, char *argv[]) {
	if (argc != 3) {
		printf("Usage: %s <ipv6> <port>\n", argv[0]);
		return 1;
	}
	char *ipv6 = argv[1];
	char *port_str = argv[2];
	int port = atoi(port_str);
	struct in6_addr addr6;
	if (inet_pton(AF_INET6, ipv6, &addr6) != 1) {
		printf("Invalid IPv6 address: %s\n", ipv6);
		return 1;
	}
	if (port < 1 || port > 65535) {
		printf("Invalid port: %d\n", port);
		return 1;
	}
	printf("IPv6: %s\nPort: %d\n", ipv6, port);
	return 0;
}
