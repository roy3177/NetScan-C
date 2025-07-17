/**
 * @author Roy Meoded
 * @date 12/07/2025
 * roymeoded2512@gmail.com
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/port_utils.h"
#include "../include/services.h"
#include "../include/parallel_scan.h"




int main(int argc, char *argv[]) {
    if (argc != 4) {
        fprintf(stderr, "Usage: %s <IP> <START_PORT> <END_PORT>\n", argv[0]);
        return 1;
    }

    const char *ip = argv[1];
    int start_port = atoi(argv[2]);
    int end_port = atoi(argv[3]);

    

    printf("Scanning %s from port %d to %d...\n", ip, start_port, end_port);

   
    int batch_size = 100; // You can adjust the batch size as needed.
    scan_ports_parallel(ip, start_port, end_port, batch_size);
 
    return 0;
}
