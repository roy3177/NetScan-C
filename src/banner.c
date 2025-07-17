#include "banner.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h> 
#include <sys/time.h>
#include "../include/banner.h"


//Try to receive a banner from the open socket:
void grab_banner(int sock, int port, char* buffer_out){
    char buffer[1024]= {0}; //Buffer to hold the banner.

    const char *msg = NULL;

    // Send a specific protocol request based on the port number
    switch(port) {
        case 21: msg = "HELP\r\n"; break;               // FTP
        case 22: break;                                 // SSH usually replies immediately
        case 23: msg = "\r\n"; break;                   // Telnet
        case 25: msg = "HELO example.com\r\n"; break;   // SMTP
        case 80:
        case 8080: msg = "HEAD / HTTP/1.0\r\n\r\n"; break; // HTTP
        case 110: msg = "USER test\r\n"; break;         // POP3
        case 143: msg = "a1 CAPABILITY\r\n"; break;     // IMAP
        case 3306: break;                               // MySQL may return a binary banner
        default: break;
    }

    // Send the request if defined
    if (msg) {
        send(sock, msg, strlen(msg), 0);
    }

    // Wait for banner response after sending the HTTP request:
    fd_set readfds;
    FD_ZERO(&readfds);
    FD_SET(sock, &readfds);

    // Set a timeout for receiving the banner
    struct timeval timeout;
    timeout.tv_sec = 2;
    timeout.tv_usec = 0;
    setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout));

   // Wait until the socket is ready to read:
    int ready = select(sock + 1, &readfds, NULL, NULL, &timeout);
    if (ready > 0 && FD_ISSET(sock, &readfds)) {
        int bytes_received = recv(sock, buffer, sizeof(buffer) - 1, 0);
        if (bytes_received > 0) {
            buffer[bytes_received] = '\0';
            printf("    [BANNER] %s\n", buffer);
        } 
        else {
            printf("    [BANNER] (no response or empty)\n");
        }
    } 
    else {
        printf("    [BANNER] (timeout waiting for response)\n");
    }
}