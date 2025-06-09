#include "tcp_header.h"
#include <stdio.h>
#include <string.h>
#ifndef MAXBUFFER
#define MAXBUFFER 4096
#endif

int tcp_client(const char* hostname, const char* port){
    struct addrinfo hints;
    memset(&hints,0,sizeof(hints));
    hints.ai_family=AF_UNSPEC;
    hints.ai_socktype=SOCK_STREAM;
    struct addrinfo* peer_address;
    int getaddrinfo_res=getaddrinfo(hostname,port,&hints,&peer_address);
    if(getaddrinfo_res!=0){
        if(getaddrinfo_res==EAI_SYSTEM){
            fprintf(stderr,"getaddrinfo() failed. %s, error code %d.\n", strerror(errno),errno);
        }else{
            fprintf(stderr,"getaddrinfo() failed. %s, error code %d.\n", gai_strerror(getaddrinfo_res),getaddrinfo_res);
        }
        exit(errno);
    }
    
    int socket_peer=socket(peer_address->ai_family, peer_address->ai_socktype,peer_address->ai_protocol);
    if(socket_peer<0){
        fprintf(stderr,"socket() failed. %s, error code %d.\n",strerror(errno),errno);
        exit(errno);
    }

    int connect_res=connect(socket_peer,peer_address->ai_addr, peer_address->ai_addrlen);
    if(connect<0){
        fprintf(stderr,"connect() failed. %s, error code %d.\n",strerror(errno),errno);
        exit(errno);
    }
    freeaddrinfo(peer_address);

    while(1){
        fd_set reads;
        FD_ZERO(&reads);
        FD_SET(socket_peer,&reads);
        
        if(select(socket_peer+1,&reads,0,0,0)<0){
            fprintf(stderr,"select() failed. %s, error code %d.\n",strerror(errno),errno);
            exit(errno);
        }
        
        if(FD_ISSET(socket_peer,&reads)){
            char read_buffer[MAXBUFFER];
            int bytes_received=recv(socket_peer,read,MAXBUFFER,0);
            if(bytes_received<1){
                printf("Connection closed.\n");
                break;
            }
        }

        if(FD_ISSET(0,&reads)){
            char read_buffer[MAXBUFFER];
            if(!fgets(read,MAXBUFFER,stdin)){
                break;
            }
        }
    }
}

