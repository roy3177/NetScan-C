/**
 * @author Roy Meoded
 * @date 12/07/2025
 * roymeoded2512@gmail.com
 * 
 * Contains the get_service_name function which maps port numbers to known service
 * names (e.g., 80 → HTTP, 22 → SSH, etc.).
 */



#ifndef SERVICES_H
#define SERVICES_H

const char* get_service_name(int port);

#endif 