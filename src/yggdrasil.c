#include "yggdrasil.h"

// Start service
int start_yggdrasil(){
   if(system("sudo systemctl start yggdrasil") == 0) {
       return SUCCESS;
   }
   perror("Failed to start yggdrasil\n");
   return DEFAULT_ERROR;
}

// Stop service
int stop_yggdrasil(){
   if(system("sudo systemctl stop yggdrasil") == 0) {
       return SUCCESS;
   }
   perror("Failed to stop yggdrasil\n");
   return DEFAULT_ERROR;
}

// Check status
int check_status_yggdrasil() {
    int ret = system("systemctl is-active --quiet yggdrasil");
    if (ret == 0) {
        return ACTIVE; // Active
    } else if (ret == -1) {
        return SYSTEM_ERROR; // System call failed
    }
    return INACTIVE; // Not active
}

int get_self_ipv6(char *ipv6, size_t bufsize) {
   FILE *fp = popen("sudo yggdrasil -address -useconffile /etc/yggdrasil/yggdrasil.conf", "r");
    if (fp == NULL) {
        perror("popen failed");
        return DEFAULT_ERROR;
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
            return INVALID_IPV6;
        }
    }
    pclose(fp);
    return SUCCESS;
}


