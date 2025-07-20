/**
 * @author Roy Meoded
 * @date 12/07/2025
 * roymeoded2512@gmail.com
 * 
 * Contains the grab_banner function, which performs banner grabbing — after detecting an open port,
 *  it tries to read an initial message (like HTTP header or SSH version) to identify the running service.
 */



#ifndef BANNER_H
#define BANNER_H

void grab_banner(int sock, int port, char* buffer_out);

#endif

