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

    socket socket_listen=socket(bind_address->ai_family, bind_address->ai_socktype, bind_address->ai_protocol);
    if(socket_listen==-1){
        fprintf(stderr, "socket() failed. %s, error code %d.\n", strerror(errno),errno);
    }

    if(bind(socket_listen,bind_address->ai_addr,bind_address->ai_addrlen)){
        fprintf(stderr, "bind() failed. %s, error code %d.\n", strerror(errno),errno);
    }
    freeaddrinfo(bind_address);

    if(listen(socket_listen,10)==-1){
        fprintf(stderr, "listen() failed. %s, error code %d.\n", strerror(errno),errno);
    }

    
}