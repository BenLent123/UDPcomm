//gcc -Wall -o parse parse.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define CONFIG_FILE "/etc/yggdrasil/yggdrasil.conf"

FILE *open_yggdrasil_config(){
   FILE *fp = fopen(CONFIG_FILE, "r");
    if (!fp) {
        perror("Failed to open config file");
        return NULL;
    }
    return fp;
}

int close_yggdrasil_config(FILE *fp){
    if (fclose(fp) == EOF) {
    perror("fclose failed");
    return -1;
    }
    return 0;
}


// Generalized function to parse and print items for a given key (e.g., "Peers")
int parse_word(FILE *fp, const char *word) {
    char line[512];
    int in_section = 0;
    while (fgets(line, sizeof(line), fp)) {
        if (!in_section) {
            char *pos = strstr(line, word);
            if (pos && strchr(line, ':') ) {
                printf("Found '%s' section:\n", word);
                in_section = 1;
                // Print any items on the same line
                char *start = strchr(line, '[');
                char *end = strchr(line, ']');
                if (start && end && end > start + 1) {
                    char items_line[512];
                    strncpy(items_line, start + 1, end - start - 1);
                    items_line[end - start - 1] = '\0';
                    char *token = strtok(items_line, ",");
                    while (token) {
                        while (*token == ' ' || *token == '"' || *token == '\'') token++;
                        char *tend = token + strlen(token) - 1;
                        while (tend > token && (*tend == ' ' || *tend == '"' || *tend == '\'')) {
                            *tend = '\0';
                            tend--;
                        }
                        if (*token)
                            printf("  %s\n", token);
                        token = strtok(NULL, ",");
                    }
                    break;
                }
            }
        } else {
            if (strchr(line, ']')) {
                break;
            }
            char *trim = line;
            while (*trim == ' ' || *trim == '\t') trim++;
            if (*trim == '"' || *trim == '\'') {
                char *end = trim + strlen(trim) - 1;
                while (end > trim && (*end == ',' || *end == '"' || *end == '\'' || *end == '\n')) {
                    *end = '\0';
                    end--;
                }
                if (*trim == '"' || *trim == '\'') trim++;
                printf("  %s\n", trim);
            }
        }
    }
    return 0;
}

// Add a peer to the Peers array in the config file
int add_peer(const char *peer_uri) {
    FILE *fp = fopen(CONFIG_FILE, "r");
    if (!fp) {
        perror("Failed to open config file for reading");
        return -1;
    }
    FILE *tmp = fopen("/tmp/yggdrasil.conf.tmp", "w");
    if (!tmp) {
        perror("Failed to open temp file for writing");
        fclose(fp);
        return -1;
    }
    char line[512];
    int in_peers = 0, added = 0;
    while (fgets(line, sizeof(line), fp)) {
        if (!in_peers) {
            fputs(line, tmp);
            if (strstr(line, "Peers") && strchr(line, ':') && strchr(line, '[')) {
                in_peers = 1;
                // Check if Peers is single-line
                if (strchr(line, ']')) {
                    // Insert before closing ]
                    char *end = strchr(line, ']');
                    if (end > line) {
                        // Move back to before ]
                        fseek(tmp, -1, SEEK_CUR);
                        fprintf(tmp, ",\n    \"%s\"]\n", peer_uri);
                        added = 1;
                        break;
                    }
                }
            }
        } else {
            // Find the closing ]
            if (strchr(line, ']')) {
                // Insert before ]
                fprintf(tmp, "    \"%s\",\n", peer_uri);
                fputs(line, tmp);
                added = 1;
                in_peers = 0;
            } else {
                fputs(line, tmp);
            }
        }
    }
    // Copy rest of file
    while (fgets(line, sizeof(line), fp)) {
        fputs(line, tmp);
    }
    fclose(fp);
    fclose(tmp);
    if (added) {
        rename("/tmp/yggdrasil.conf.tmp", CONFIG_FILE);
        printf("Peer added: %s\n", peer_uri);
        return 0;
    } else {
        remove("/tmp/yggdrasil.conf.tmp");
        printf("Failed to add peer.\n");
        return -1;
    }
}

// Delete a peer from the Peers array in the config file
int delete_peer(const char *peer_uri) {
    FILE *fp = fopen(CONFIG_FILE, "r");
    if (!fp) {
        perror("Failed to open config file for reading");
        return -1;
    }
    FILE *tmp = fopen("/tmp/yggdrasil.conf.tmp", "w");
    if (!tmp) {
        perror("Failed to open temp file for writing");
        fclose(fp);
        return -1;
    }
    char line[512];
    int in_peers = 0, deleted = 0;
    while (fgets(line, sizeof(line), fp)) {
        if (!in_peers) {
            fputs(line, tmp);
            if (strstr(line, "Peers") && strchr(line, ':') && strchr(line, '[')) {
                in_peers = 1;
            }
        } else {
            if (strchr(line, ']')) {
                fputs(line, tmp);
                in_peers = 0;
            } else {
                // Check if this line contains the peer_uri
                if (strstr(line, peer_uri)) {
                    deleted = 1;
                    continue; // skip this line
                } else {
                    fputs(line, tmp);
                }
            }
        }
    }
    // Copy rest of file
    while (fgets(line, sizeof(line), fp)) {
        fputs(line, tmp);
    }
    fclose(fp);
    fclose(tmp);
    if (deleted) {
        rename("/tmp/yggdrasil.conf.tmp", CONFIG_FILE);
        printf("Peer deleted: %s\n", peer_uri);
        return 0;
    } else {
        remove("/tmp/yggdrasil.conf.tmp");
        printf("Peer not found.\n");
        return -1;
    }
}

// List peers using libyaml


int main(void){
    FILE *fp = open_yggdrasil_config();
    if (!fp) {
        perror("open issue");
        return -1;
    }

    parse_word(fp,"InterfacePeers");
    printf("custom done\n");
    
    close_yggdrasil_config(fp);
    printf("end\n");
    return 0;
}