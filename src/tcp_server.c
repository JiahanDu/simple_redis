#include "tcp_header.h"
#include <stdio.h>
#include <string.h>

int tcp_server(char* port){
    struct addrinfo hints;
    memset(&hints,0,sizeof(hints))
    hints.ai_family=AF_INET;
    hints.ai_socktype=SOCK_STREAM;
    hints.ai_flag=AI_PASSIVE;

    struct addrinfo* bind_address;
    getaddrinfo(0, port, &hints, &bind_address);
    
}