#ifndef UTILS_H
#define UTILS_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <string.h>

#define PORT 8080
#define BUFFER_SIZE 4096
#define MAX_CLIENTS 4000

int check(int exp, const char* msg);
void handle_connection(int client_socket);

#endif /* ifndef UTILS_H */
