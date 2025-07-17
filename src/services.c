#include "../include/services.h"

/*
    * Function to get the service name based on the port number.
    * @param port The port number.
    * @return The name of the service or "Unknown Service" if not recognized.
*/

const char* get_service_name(int port) {
    switch (port) {
        case 20: return "FTP Data Transfer";
        case 21: return "FTP Control";
        case 22: return "SSH";
        case 23: return "Telnet";
        case 25: return "SMTP";
        case 53: return "DNS";
        case 80: return "HTTP";
        case 110: return "POP3";
        case 143: return "IMAP";
        case 443: return "HTTPS";
        case 3306: return "MySQL";
        default: return "Unknown Service";
    }
}
