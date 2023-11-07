#include "utils.h"

#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdbool.h>
#include <pthread.h>
#include <sys/select.h>
#include <sys/ioctl.h>

bool handle_select_connection(int client_socket);

int main() {
  int server_socket, client_socket, addr_size;
  addr_size = sizeof(struct sockaddr_in);

  struct sockaddr_in server_addr, client_addr;

  check((server_socket = socket(AF_INET, SOCK_STREAM, 0)), "failed to create socket");

  server_addr.sin_family = AF_INET;
  server_addr.sin_addr.s_addr = INADDR_ANY;
  server_addr.sin_port = htons(PORT);

  check(bind(server_socket, (struct sockaddr*)&server_addr, sizeof(server_addr)), "bind failed!");

  check(listen(server_socket, MAX_CLIENTS), "listen failed");

  printf("Server is listening on port %d...\n", PORT);

  fd_set current_sockets, ready_sockets;
  FD_ZERO(&current_sockets);
  FD_SET(server_socket, &current_sockets);

  while (true) {
    /*printf("Waiting for connection...\n");*/
    ready_sockets = current_sockets;

    check(select(MAX_CLIENTS, &ready_sockets, NULL, NULL, NULL), "select failed");

    for (int i = 0; i < MAX_CLIENTS; i++) {
      if (FD_ISSET(i, &ready_sockets)) {
        if (i == server_socket) {
          check(client_socket = accept(server_socket, (struct sockaddr*)&client_addr, (socklen_t*)&addr_size), "accept failed");
          FD_SET(client_socket, &current_sockets);
        } else {
          bool close = handle_select_connection(i);
          if (close) FD_CLR(i, &current_sockets);
        }
      }
    }
  }

  return 0;
}

bool handle_select_connection(int client_socket) {
    char buffer[BUFFER_SIZE];
    char result[20];
    int valread;
    uint64_t n;


    int iMode = 0;
    ioctl(client_socket, FIONBIO, &iMode);
    valread = read(client_socket, buffer, BUFFER_SIZE);
    if (valread > 0) {
        /*printf("Received: %s\n", buffer);*/
        n = atoi(buffer);
        long fact = factorial(n);
        sprintf(result, "%ld", fact);
        /*printf("Sending: %s\n", result);*/
        write(client_socket, result, strlen(result));
        return false;
    }


    close(client_socket);
    return true;
}
