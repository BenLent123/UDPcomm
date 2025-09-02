#ifndef YGGDRASIL_H
#define YGGDRASIL_H

#ifdef __cplusplus
extern "C" {
#endif

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