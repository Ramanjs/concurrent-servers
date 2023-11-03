#include "utils.h"
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdbool.h>
#include <poll.h>

bool handle_poll_connection(int client_socket);

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

    struct pollfd fds[MAX_CLIENTS];
    int nfds = 1;  // Initially only server_socket

    fds[0].fd = server_socket;
    fds[0].events = POLLIN;

    while (true) {
        check(poll(fds, nfds, -1), "poll failed");

        for (int i = 0; i < nfds; i++) {
            if (fds[i].revents & POLLIN) {
                if (fds[i].fd == server_socket) {
                    check(client_socket = accept(server_socket, (struct sockaddr*)&client_addr, (socklen_t*)&addr_size), "accept failed");
                    fds[nfds].fd = client_socket;
                    fds[nfds].events = POLLIN;
                    nfds++;
                    /*printf("New connection accepted.\n");*/
                } else {
                    bool closed = handle_poll_connection(fds[i].fd);
                    if (closed) {
                        close(fds[i].fd);
                        // Remove the closed socket from the array and adjust nfds
                        for (int j = i; j < nfds - 1; j++) {
                            fds[j] = fds[j + 1];
                        }
                        nfds--;
                    }
                }
            }
        }
    }

    return 0;
}

bool handle_poll_connection(int client_socket) {
    char buffer[BUFFER_SIZE];
    char result[20];
    int valread;
    uint64_t n;

    valread = read(client_socket, buffer, BUFFER_SIZE);
    if (valread > 0) {
        n = atoi(buffer);
        long fact = factorial(n);
        sprintf(result, "%ld", fact);
        write(client_socket, result, strlen(result));
        return false;
    }

    return true;
}

