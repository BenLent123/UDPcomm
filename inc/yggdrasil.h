#ifndef YGGDRASIL_H
#define YGGDRASIL_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <arpa/inet.h>

#define SUCCESS 0
#define INVALID_ADDR6 -1
#define ERROR -2
#define INACTIVE -3
#define ACTIVE 1

// Starts the Yggdrasil service
int start_yggdrasil(void);

// Stops the Yggdrasil service
int stop_yggdrasil(void);

// Checks the status of the Yggdrasil service
int check_status_yggdrasil(void);

// Gets the Yggdrasil IPv6 address of the current node
int get_self_ipv6(char *buf, size_t bufsize);

#ifdef __cplusplus
}
#endif

#endif // YGGDRASIL_H