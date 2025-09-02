#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <arpa/inet.h>
#include "yggdrasil.h"

// Start service
int start_yggdrasil(){
   if(system("sudo systemctl start yggdrasil") == 0) return 0;
   return -1;
}

// Stop service
int stop_yggdrasil(){
   if(system("sudo systemctl stop yggdrasil") == 0) return 0;
   return -1;
}

// Check status
int check_status_yggdrasil() {
    if (system("systemctl is-active --quiet yggdrasil") == 0)
        return 0; // Active
    return -1;    // Not active (dead/inactive)
}

int get_self_ipv6(char *ipv6, size_t bufsize) {
   FILE *fp = popen("sudo yggdrasil -address -useconffile /etc/yggdrasil/yggdrasil.conf", "r");
    if (fp == NULL) {
        perror("popen failed");
        return 1;
    }

    if (fgets(ipv6, bufsize, fp) != NULL) {
        // Remove trailing newline if present
        size_t len = strlen(ipv6);
        if (len > 0 && ipv6[len - 1] == '\n') {
            ipv6[len - 1] = '\0';
        }
        struct in6_addr addr6;
        if (inet_pton(AF_INET6, ipv6, &addr6) == 1) {
            printf("Valid IPv6 address: %s\n", ipv6);
        } else {
            printf("Invalid IPv6 address: %s\n", ipv6);
        }
    }
    pclose(fp);
    return 0;
}


