#include "hashmap.h"
#include <string.h>

unsigned long hash(char* key){
    unsigned long hash=5381;
    int c;

    while((c=*key++)){
        hash=hash*33+c;
    }

    return hash;
}

size_t size(HashMap* dict){
    return dict->size;
}

void add(HashMap* dict, char* key, void* val, Type type, size_t val_size){
    unsigned long num=hash(key)%dict->capacity;
    HashEntry* cur=dict->buckets[num];
    while(cur->next){
        cur=cur->next;
        if(strcmp(cur->key,key)==0){
            void* cpy=malloc(val_size);
            memcpy(cpy,val,val_size);
            free(cur->val);
            cur->val=cpy;
            cur->type=type;
            cur->val_size=val_size;
            return;
        }
    }
    HashEntry* node=malloc(sizeof(HashEntry));
    if(!node){
        fprintf(stderr,"malloc() failed. %s, error code %d.\n", strerror(errno),errno);
        exit(errno);
    }
    node->key=strdup(key);
    if (!node->key) {
        fprintf(stderr, "strdup() failed. %s, error code %d.\n", strerror(errno), errno);
        exit(errno);
    }
    node->val=malloc(val_size);
    if(!node->val){
        fprintf(stderr,"malloc() failed. %s, error code %d.\n", strerror(errno),errno);
        exit(errno);
    }
    memcpy(node->val,val,val_size);
    node->val_size=val_size;
    node->next=NULL;
    cur->next=node;

    dict->size+=1;
    if(dict->size>RESIZE_FACTOR*dict->capacity){
        resize(dict);
    }
}

HashMap* initialize(size_t capacity){
    if(capacity<10){
        printf("capacity must be at least 10.\n");
        exit(1);
    }
    HashMap* dict=malloc(sizeof(HashMap));
    if(!dict){
        fprintf(stderr,"malloc() failed. %s, error code %d.\n", strerror(errno),errno);
        exit(errno);
    }
    dict->size=0;
    dict->capacity=capacity;
    dict->buckets=malloc(sizeof(HashEntry*)*capacity);
    if(!dict->buckets){
        fprintf(stderr,"malloc() failed. %s, error code %d.\n", strerror(errno),errno);
        exit(errno);
    }
    for(size_t i=0;i<capacity;i++){
        dict->buckets[i]=malloc(sizeof(HashEntry));
        if(!dict->buckets[i]){
            fprintf(stderr,"malloc() failed. %s, error code %d.\n", strerror(errno),errno);
            exit(errno);
        }
        dict->buckets[i]->key=NULL;
        dict->buckets[i]->val=NULL;
        dict->buckets[i]->type=-1;
        dict->buckets[i]->val_size=0;
        dict->buckets[i]->next=NULL;
    }
    return dict;
}

bool remove(HashMap* dict, char* key){
    unsigned long num=hash(key);
    HashEntry* cur=dict->buckets[num%dict->capacity];
    while(cur->next){
        if(strcmp(cur->next->key,key)!=0){
            cur=cur->next;
        }else{
            HashEntry* temp=cur->next;
            cur->next=cur->next->next;
            destroy_entry(temp);
            dict->size-=1;
            return true;
        }
    }
    return false;
}

HashEntry* get(HashMap* dict, char* key){
    unsigned long num=hash(key);
    HashEntry* cur=dict->buckets[num%dict->capacity];
    while(cur->next){
        if(strcmp(cur->next->key,key)!=0){
            cur=cur->next;
        }else{
            return cur->next;
        }
    }
    return NULL;
}