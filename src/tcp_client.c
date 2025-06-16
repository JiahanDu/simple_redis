#include "tcp_header.h"
#ifndef MAXBUFFER
#define MAXBUFFER 4096
#endif
#ifndef MAXNUM
#define MAXNUM 20
#endif

char* command_to_RESP(char* input){
    int quotes=0;
    char res[MAXNUM];
    char temp[100];
    int res_p=0;
    int temp_p=0; 
    int count=0;
    bool escape=false;

    for(int i=0;i<strlen(input)-1;i++){
        if(input[i]=='\"'){
            if(escape){
                temp[temp_p]=input[i];
                temp_p+=1;
                escape=false;
            }else{
                if(quotes==0){
                    quotes+=1;
                }else if(quotes==1){
                    quotes-=1;
                    count+=1;
                    res[res_p]='$';
                    res_p+=1;
                    int ans=sprintf(res+res_p,"%d",temp_p);
                    res_p+=ans;
                    res[res_p]='\r';
                    res_p+=1;
                    res[res_p]='\n';
                    res_p+=1;
                    strncpy(res+res_p,temp,temp_p);
                    res_p+=temp_p;
                    res[res_p]='\r';
                    res_p+=1;
                    res[res_p]='\n';
                    res_p+=1;
                    memset(temp,0,sizeof(temp));
                    temp_p=0;
                }else{
                    return NULL;
                }
            }
        }else if(input[i]=='\\'){
            if(escape){
                temp[temp_p]=input[i];
                temp_p+=1;
                escape=false;
            }else{
                if(input[i+1]=='\"' || input[i+1]=='\\'){
                    escape=true;
                }else{
                    return NULL;
                }
            }
        }else if(input[i]=='\t' || input[i]==' '){
            if(quotes>0){
                temp[temp_p]=input[i];
                temp_p+=1;
            }else{
                continue;
            }
        }else if(input[i]!='\n'){
            if(quotes>0){
                temp[temp_p]=input[i];
                temp_p+=1;
            }else{
                temp[temp_p]=input[i];
                temp_p+=1;
                if(input[i+1]==' ' || input[i+1]=='\t' || input[i+1]=='\n'){
                    count+=1;
                    res[res_p]='$';
                    res_p+=1;
                    int ans=sprintf(res+res_p,"%d",temp_p);
                    res_p+=ans;
                    res[res_p]='\r';
                    res_p+=1;
                    res[res_p]='\n';
                    res_p+=1;
                    strncpy(res+res_p,temp,temp_p);
                    res_p+=temp_p;
                    res[res_p]='\r';
                    res_p+=1;
                    res[res_p]='\n';
                    res_p+=1;
                    memset(temp,0,sizeof(temp));
                    temp_p=0;
                }
            }
        }
    }
    int ans_p=0;
    char* ans=malloc(sizeof(char)*MAXBUFFER);
    ans[ans_p]='*';
    ans_p+=1;
    int num=sprintf(ans+ans_p,"%d",count);
    ans_p+=num;
    ans[ans_p]='\r';
    ans_p+=1;
    ans[ans_p]='\n';
    ans_p+=1;
    strncpy(ans+ans_p,res,res_p);
    return ans;
}

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
    ssize_t bytes_received=0;
    ssize_t bulk_string=0;
    int received_int=0;

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
            ssize_t cur=recv(socket_peer,read_buffer+bytes_received,MAXBUFFER-bytes_received,0);
            if(cur<1){
                break;
            }
            bytes_received+=cur;
            read_buffer[bytes_received]='\0';
            if(read_buffer[0]=='+' || read_buffer[0]==':'){
                char* p=strstr(read_buffer,"\r\n");  
                if(!p){
                    continue;
                }
                if(read_buffer[0]==':'){
                    received_int=strtol(read_buffer+1,NULL,10);
                }
                if(read_buffer[0]=='+'){
                    printf("%s\n",read_buffer+1);
                }else{
                    printf("%d\n",received_int);
                }

                p+=2;
                memmove(read_buffer,p,strlen(p)+1);
                bytes_received-=(p-read_buffer);
            }else if(read_buffer[0]=='$'){
                char* p=strstr(read_buffer,"\r\n");
                if(!p){
                    continue;
                }
                bulk_string=strtol(read_buffer+1,NULL,10);
                p+=2;
                if(strlen(p)<bulk_string+2){
                    continue;
                }
                printf("%s\n",p);
                memmove(read_buffer,p,strlen(p)+1);
                bytes_received-=(p-read_buffer);
            }
        }

        if(FD_ISSET(0,&reads)){
            char read_buffer[MAXBUFFER];
            if(!fgets(read_buffer,MAXBUFFER,stdin)){
                break;
            }
            char* request_buffer=command_to_RESP(MAXBUFFER);
            if(!request_buffer){
                printf("Invalid Syntax.\n");
                exit(1);
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