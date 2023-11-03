#include "utils.h"

#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdbool.h>

int main(int argc, char** argv) {
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

  while (true) {
    /*printf("Waiting for connection...\n");*/

    check(client_socket = accept(server_socket, (struct sockaddr*)&client_addr, (socklen_t*)&addr_size), "accept failed");

    /*printf("Connected!\n");*/

    handle_connection(client_socket);
  }

  return 0;
}
