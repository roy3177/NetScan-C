#include "../include/port_utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/select.h>

/**
 * Function to check if a port is open on a given IP address.
 * @param ip The IP address to check.
 * @param port The port number to check.
 * @return 1 if the port is open, 0 if closed, -1 if timeout occurred.
 */
int is_port_open(const char *ip, int port) {
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        perror("Socket creation failed");
        return 0;
    }

    fcntl(sock, F_SETFL, O_NONBLOCK);

    struct sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);

    if (inet_pton(AF_INET, ip, &server_addr.sin_addr) <= 0) {
        perror("Invalid address");
        close(sock);
        return 0;
    }

    connect(sock, (struct sockaddr *)&server_addr, sizeof(server_addr));

    fd_set fdset;
    FD_ZERO(&fdset);
    FD_SET(sock, &fdset);

    struct timeval timeout;
    timeout.tv_sec = 0;
    timeout.tv_usec = 300000;

    int ready = select(sock + 1, NULL, &fdset, NULL, &timeout);

    int result = 0;
    if (ready > 0) {
        int so_error;
        socklen_t len = sizeof so_error;
        getsockopt(sock, SOL_SOCKET, SO_ERROR, &so_error, &len);
        result = (so_error == 0) ? 1 : 0;  // 1 = open, 0 = closed
    } 
    else if (ready == 0) {
        result = -1; // timeout
    } 
    else {
        perror("select error");
        result = 0; // treat as closed on error
    }

    close(sock);
    return result;
}

