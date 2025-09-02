#include <stdlib.h>
#include <stdio.h>
#include <string.h>
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

int get_self_ipv6(char *buf, size_t bufsize) {
    FILE *fp = popen("sudo yggdrasilctl getself", "r");
    if (fp == NULL) {
        return -1;
    }

    char line[256];
    while (fgets(line, sizeof(line), fp) != NULL) {
        if (strncmp(line, "IPv6 address:", 13) == 0) {
            // Copy everything after "IPv6 address: " into buf
            const char *addr = line + 14; // skip "IPv6 address: "
            // Remove trailing newline if present
            size_t len = strcspn(addr, "\n");
            if (len >= bufsize) len = bufsize - 1;
            strncpy(buf, addr, len);
            buf[len] = '\0';
            pclose(fp);
            return 0;
        }
    }

    pclose(fp);
    return -1; // IPv6 address not found
}
