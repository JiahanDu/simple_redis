#include "tcp_header.h"
#include <stdio.h>
#include <string.h>
#ifndef MAXEVENTS
#define MAXEVENTS 10
#endif
#ifndef MAXBUFFER
#define MAXBUFFER 4096
#endif

int tcp_server(char* port){
    struct addrinfo hints;
    memset(&hints,0,sizeof(hints));
    hints.ai_family=AF_INET;
    hints.ai_socktype=SOCK_STREAM;
    hints.ai_flags=AI_PASSIVE;

    struct addrinfo* bind_address;
    int s=getaddrinfo(0, port, &hints, &bind_address);
    if(s!=0){
        if(s==EAI_SYSTEM){
            fprintf(stderr,"getaddrinfo() failed. %s, error code %d.\n", strerror(errno),errno);
        }else{
            fprintf(stderr,"getaddrinfo() failed. %s, error code %d.\n", gai_strerror(s),s);
        }
        exit(s);
    }

    int socket_listen=socket(bind_address->ai_family, bind_address->ai_socktype, bind_address->ai_protocol);
    if(socket_listen==-1){
        fprintf(stderr, "socket() failed. %s, error code %d.\n", strerror(errno),errno);
        exit(errno);
    }

    if(bind(socket_listen,bind_address->ai_addr,bind_address->ai_addrlen)){
        fprintf(stderr, "bind() failed. %s, error code %d.\n", strerror(errno),errno);
        exit(errno);
    }
    freeaddrinfo(bind_address);

    if(listen(socket_listen,10)==-1){
        fprintf(stderr, "listen() failed. %s, error code %d.\n", strerror(errno),errno);
        exit(errno);
    }

    int epfd=epoll_create1(0);
    if(epfd==-1){
        fprintf(stderr, "epoll_create1() failed. %s, error code %d.\n", strerror(errno),errno);
        exit(errno);
    }

    struct epoll_event ev;
    ev.events=EPOLLIN;
    ev.data.fd=socket_listen;
    if(epoll_ctl(epfd,EPOLL_CTL_ADD,socket_listen,&ev)==-1){
        fprintf(stderr, "epoll_ctl() failed. %s, error code %d.\n", strerror(errno),errno);
        exit(errno);
    }
    struct epoll_event events[MAXEVENTS];
    struct sockaddr_in client_address;
    while(1){
        socklen_t address_len=sizeof(client_address);
        int nfds=epoll_wait(epfd,events,MAXEVENTS,-1);
        if(nfds==-1){
            fprintf(stderr, "epoll_wait() failed. %s, error code %d.\n", strerror(errno),errno);
            exit(errno);
        }
        for(int i=0;i<nfds;i++){
            if(events[i].data.fd==socket_listen){
                int socket_connect=accept(socket_listen,(struct sockaddr*)&client_address, &address_len);
                if(socket_connect==-1){
                    fprintf(stderr, "accept() failed. %s, error code %d.\n", strerror(errno),errno);
                    exit(errno);
                }
                
            }else{
                char buffer[MAXBUFFER];
                ssize_t read_len=read(events[i].data.fd,buffer,MAXBUFFER);
                if(read_len==-1){
                    fprintf(stderr, "read() failed. %s, error code %d.\n", strerror(errno),errno);
                    exit(errno);
                }else if(read_len==0){
                    printf("Connection closed.\n");
                    close(events[i].data.fd);
                }
            }
        }
    }
}