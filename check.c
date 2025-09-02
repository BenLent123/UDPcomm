#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <arpa/inet.h>

char *get_self() {
    FILE *fp = popen("yggdrasilctl getself", "r");
    if (fp == NULL) return NULL;
    char line[256];
    while (fgets(line, sizeof(line), fp) != NULL) {
        if (strncmp(line, "IPv6 address:", 13) == 0) {
            const char *addr = line + 14;
            size_t len = strcspn(addr, "\n");
            char *out = malloc(len + 1);
            if (!out) { pclose(fp); return NULL; }
            strncpy(out, addr, len);
            out[len] = '\0';
            // Trim whitespace
            char *start = out;
            while (*start && isspace((unsigned char)*start)) start++;
            char *end = start + strlen(start) - 1;
            while (end > start && isspace((unsigned char)*end)) end--;
            *(end + 1) = '\0';
            if (start != out) memmove(out, start, strlen(start) + 1);
            pclose(fp);
            return out;
        }
    }
    pclose(fp);
    return NULL;
}

int main() {
    char *ipv6 = get_self();
    struct in6_addr addr6;

    if (ipv6) {
        printf("%s\n", ipv6);
        if (inet_pton(AF_INET6, ipv6, &addr6) == 1) {
            printf("Valid IPv6 address.\n");
        } else {
            printf("Invalid IPv6 address.\n");
        }
        free(ipv6);
    } else {
        printf("Could not get IPv6 address.\n");
    }
    return 0;
}
