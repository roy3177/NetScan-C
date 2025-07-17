
#include "../include/port_utils.h"
#include "../include/services.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "../include/banner.h"
#include <fcntl.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <sys/select.h>
#include <time.h>


/*
* Function to scan ports in parallel.
* @param ip The IP address to scan.
* @param start_port The starting port number.
* @param end_port The ending port number.
* @param batch_size The number of ports to scan in parallel.
*/

void scan_ports_parallel(const char *ip, int start_port, int end_port, int batch_size) {

    FILE* html_report=fopen("report.html", "w");
    if(!html_report) {
        perror("Failed to open report file");
        exit(1);
    }

    int open_count = 0;
    int closed_count = 0;
    int timeout_count = 0;

    //Write the HTML header to the report file:
    fprintf(html_report, "<!DOCTYPE html><html><head><meta charset='UTF-8'>\n");
    fprintf(html_report, "<title>Port Scan Report</title>\n");
    fprintf(html_report, "<style>body{font-family:sans-serif;} table{border-collapse:collapse;} th,td{padding:8px;border:1px solid #ccc;} .open{color:green;} .closed{color:red;} .timeout{color:orange;}</style>\n");
    fprintf(html_report, "</head><body>\n");
    fprintf(html_report, "<h2>Scan Report for %s</h2>\n", ip);
    fprintf(html_report, "<table>\n<tr><th>Port</th><th>Status</th><th>Service</th><th>Banner</th></tr>\n");

    for (int i = start_port; i <= end_port; i += batch_size) {
        int sockets[batch_size]; //Array to hold socckets.
        struct sockaddr_in addrs[batch_size]; //Array to holds destination addresses(for any port).
    

        fd_set fdset; //Set of sockets, that the select function will monitor.
        FD_ZERO(&fdset);
        int maxfd=-1; 

        //Check if the last batch is smaller than the batch size:
        int actual_batch = (i + batch_size <= end_port) ? batch_size : (end_port - i + 1);

        //Loop through the ports in the current batch:
        for (int j = 0; j < actual_batch; j++) {
            int port = i + j;
            int sock = socket(AF_INET, SOCK_STREAM, 0);
            if (sock < 0) {
                continue;
            }

            //Set the socket to non-blocking mode:
            fcntl(sock, F_SETFL, O_NONBLOCK);

            //Ready the address structure for the current port:
            memset(&addrs[j], 0, sizeof(addrs[j]));
            addrs[j].sin_family = AF_INET;
            addrs[j].sin_port = htons(port);
            inet_pton(AF_INET, ip, &addrs[j].sin_addr);

            //Connect the socket to the address:
            connect(sock, (struct sockaddr*)&addrs[j], sizeof(addrs[j]));
            FD_SET(sock, &fdset); //Add the socket to the seleect set.
            sockets[j] = sock; //Store the socket in the array.
            if (sock > maxfd){ 
                maxfd = sock;
            }
        }

        //Wait for the sockets to become ready:
        struct timeval timeout;
        timeout.tv_sec = 0;
        timeout.tv_usec = 300000;

        //Use select to monitor the sockets:
        select(maxfd + 1, NULL, &fdset, NULL, &timeout);

        //Check the status of each socket in the batch:
        for (int j = 0; j < actual_batch; j++) {
            int sock = sockets[j];
            int port = i + j;

            //Check if the socket is ready:
            if (FD_ISSET(sock, &fdset)) {
                int err;
                socklen_t len = sizeof(err);

                //Get the socket error status:
                getsockopt(sock, SOL_SOCKET, SO_ERROR, &err, &len);
                if (err == 0) {
                    printf("Port %d (%s) is OPEN\n", port, get_service_name(port));
                    open_count++; // Increment the open port count
                    
                    char banner_buffer[1024] = {0};
                    grab_banner(sock, port, banner_buffer); 

                    fprintf(html_report,
                        "<tr><td>%d</td><td class='open'>OPEN</td><td>%s</td><td>%s</td></tr>\n",
                        port, get_service_name(port),
                        strlen(banner_buffer) > 0 ? banner_buffer : "N/A");


                } 
                else {
                    printf("Port %d (%s) is CLOSED\n", port, get_service_name(port));
                    closed_count++; // Increment the closed port count
                    fprintf(html_report,
                    "<tr><td>%d</td><td class='closed'>CLOSED</td><td>%s</td><td>N/A</td></tr>\n",
                    port, get_service_name(port));
                
                }
            } 
            else {
                printf("Port %d (%s) is UNRESPONSIVE (TIMEOUT)\n", port, get_service_name(port));
                timeout_count++; // Increment the timeout count

                fprintf(html_report,
                    "<tr><td>%d</td><td class='timeout'>TIMEOUT</td><td>%s</td><td>N/A</td></tr>\n",
                    port, get_service_name(port));
            }
            close(sock);
        }
    }

    fprintf(html_report, "</table>\n<p>Report generated on %s</p>", __DATE__);
    fprintf(html_report, "</body></html>\n");
    fprintf(html_report, "<p><strong>Summary:</strong> %d open, %d closed, %d timeout</p>\n", open_count, closed_count, timeout_count);

    fclose(html_report);

}