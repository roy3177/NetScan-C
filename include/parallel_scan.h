/**
 * @author Roy Meoded
 * @date 12/07/2025
 * roymeoded2512@gmail.com
 * 
 * Contains the scan_ports_parallel function that scans ports in parallel using select()
 * to detect open/closed/unresponsive ports. It also generates a detailed HTML report of the scan results.
 */


#ifndef PARALLEL_SCAN_H
#define PARALLEL_SCAN_H



void scan_ports_parallel(const char *ip, int start_port, int end_port, int batch_size);

#endif