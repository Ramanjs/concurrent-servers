#ifndef UTILS_H
#define UTILS_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <string.h>

#define PORT 8080
#define BUFFER_SIZE 2048
#define MAX_CLIENTS 3001

int check(int exp, const char* msg);
long factorial(uint64_t n);
void *handle_connection(void* p_client_socket);

#endif /* ifndef UTILS_H */
