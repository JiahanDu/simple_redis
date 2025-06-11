#include "tcp_header.h"
#ifndef MAXBUFFER
#define MAXBUFFER 4096
#endif
#ifndef MAXNUM
#define MAXNUM 20
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
    if(connect_res<0){
        fprintf(stderr,"connect() failed. %s, error code %d.\n",strerror(errno),errno);
        exit(errno);
    }
    freeaddrinfo(peer_address);

    char read_buffer[MAXBUFFER];
    int bytes_received=0;
    char read_result[MAXBUFFER];
    int received_int;
    int bulk_string=0;

    while(1){
        fd_set reads;
        FD_ZERO(&reads);
        FD_SET(socket_peer,&reads);
        FD_SET(0,&reads);
        
        if(select(socket_peer+1,&reads,0,0,0)<0){
            fprintf(stderr,"select() failed. %s, error code %d.\n",strerror(errno),errno);
            exit(errno);
        }
        
        if(FD_ISSET(socket_peer,&reads)){
            int cur=recv(socket_peer,read_buffer+bytes_received,MAXBUFFER-bytes_received,0);
            if(cur<1){
                break;
            }
            bytes_received+=cur;
            if(read_buffer[0]=='+' || read_buffer[0]==':'){
                read_buffer[bytes_received]='\0';
                char* p=strstr(read_buffer,"\r\n");  
                if(!p){
                    continue;
                }
                strncpy(read_result,read_buffer+1,p-read_buffer-1);
                read_result[p - read_buffer - 1] = '\0';
                if(read_buffer[0]==':'){
                    received_int=strtol(read_result);
                }
                printf("%s\n",read_result);
                memset(read_result,0,MAXBUFFER);

                p+=2;
                memmove(read_buffer,p,strlen(p)+1);
                bytes_received-=(p-read_buffer);
            }else if(read_buffer[0]=='$'){

            }
        }

        if(FD_ISSET(0,&reads)){
            char read_buffer[MAXBUFFER];
            if(!fgets(read_buffer,MAXBUFFER,stdin)){
                break;
            }
            char request_buffer[MAXBUFFER];
            char* p=request_buffer;
            int count=0;
            int start[MAXNUM];
            int end[MAXNUM];
            for(int i=0;i<MAXBUFFER;i++){
                if(read_buffer[i]=='\n'){
                    break;
                }
                if(read_buffer[i]!=' ' && read_buffer[i]!='\t' && (i==0|| read_buffer[i-1]==' ' || read_buffer[i-1]=='\t')){
                    start[count]=i;
                }
                if(read_buffer[i]!=' ' && read_buffer[i]!='\t' && (read_buffer[i+1]==' ' || read_buffer[i+1]=='\t' || read_buffer[i+1]=='\n')){
                    end[count]=i;
                    count+=1;
                }
            }
            int num=snprintf(p, sizeof(request_buffer),"*%d\r\n",count);
            for(int i=0;i<count;i++){
                num+=snprintf(p+num,sizeof(request_buffer)-num,"$%d\r\n",end[i]-start[i]+1);
                memcpy(p+num,read_buffer+start[i],end[i]-start[i]+1);
                num+=end[i]-start[i]+1;
                memcpy(p+num,"\r\n",2);
                num+=2;
            }
            int bytes_sent=send(socket_peer,request_buffer,num,0);
            if(bytes_sent<0){
                fprintf(stderr,"send() failed. %s, error code %d.\n",strerror(errno),errno);
                exit(errno);
            }
        }
    }
    return 0;
}

int main(int argc, char* argv[]){
    if(argc<3){
        printf("Usage example: ./tcp_client 127.0.0.1 6379.\n");
        exit(1);
    }
    tcp_client(argv[1],argv[2]);
    return 0;
}