#include "utils.h"

int check(int exp, const char* msg) {
  if (exp == -1) {
    perror(msg);
    exit(1);
  }
  return exp;
}

long factorial(uint64_t n) {
  long fact = 1;

  if (n > 20) n = 20;

  for (int i = 2; i <= n; i++) {
    fact = fact * i;
  }

  return fact;
}

void handle_connection(int client_socket) {
    char buffer[BUFFER_SIZE];
    char result[20];
    int valread;
    uint64_t n;

    while (1) {
        valread = read(client_socket, buffer, BUFFER_SIZE);
        if (valread > 0) {
            /*printf("Received: %s\n", buffer);*/
            n = atoi(buffer);
            long fact = factorial(n);
            sprintf(result, "%ld", fact);
            /*printf("Sending: %s\n", result);*/
            write(client_socket, result, strlen(result));
        } else {
            break;
        }
    }

    close(client_socket);
}
