#include "utils.h"

#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdbool.h>
#include <sys/epoll.h>

#define MAX_EVENTS 10 // Maximum number of events to be returned by epoll_wait

bool handle_epoll_connection(struct epoll_event event);

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

    int epoll_fd = epoll_create1(0);
    check(epoll_fd, "failed to create epoll instance");

    struct epoll_event event;
    event.events = EPOLLIN;
    event.data.fd = server_socket;

    check(epoll_ctl(epoll_fd, EPOLL_CTL_ADD, server_socket, &event), "failed to add server_socket to epoll");

    while (true) {
        struct epoll_event events[MAX_EVENTS];
        int num_events = epoll_wait(epoll_fd, events, MAX_EVENTS, -1); // Wait for events

        check(num_events, "error in epoll_wait");

        for (int i = 0; i < num_events; i++) {
            if (events[i].data.fd == server_socket) {
                check(client_socket = accept(server_socket, (struct sockaddr*)&client_addr, (socklen_t*)&addr_size), "accept failed");

                event.events = EPOLLIN;
                event.data.fd = client_socket;

                check(epoll_ctl(epoll_fd, EPOLL_CTL_ADD, client_socket, &event), "failed to add client_socket to epoll");
            } else {
                bool close_connection = handle_epoll_connection(events[i]);
                if (close_connection) {
                    close(events[i].data.fd);
                    epoll_ctl(epoll_fd, EPOLL_CTL_DEL, events[i].data.fd, NULL);
                }
            }
        }
    }

    close(server_socket);
    return 0;
}

bool handle_epoll_connection(struct epoll_event event) {
    char buffer[BUFFER_SIZE];
    char result[20];
    int valread;
    uint64_t n;

    valread = read(event.data.fd, buffer, BUFFER_SIZE);
    if (valread > 0) {
        n = atoi(buffer);
        long fact = factorial(n);
        sprintf(result, "%ld", fact);
        write(event.data.fd, result, strlen(result));
        return false;
    }

    return true;
}

