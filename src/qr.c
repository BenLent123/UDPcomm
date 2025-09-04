// Example: Generate QR code from IPv6 and port
#include "qr.h"
int create_qr(char *ipv6, int port){
    char qr_string[128];
    snprintf(qr_string, sizeof(qr_string), "[%s]:%d", ipv6, port);
    char cmd[256];
    snprintf(cmd, sizeof(cmd), "qrencode -o myid.png \"%s\"", qr_string);
    if(system(cmd) == 0) {
        return SUCCESS;
    }else{
        perror("Failed to generate QR code\n");
        return DEFAULT_ERROR;
    }
    
    return SUCCESS;
}