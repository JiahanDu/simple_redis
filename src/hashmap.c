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

void add(HashMap* dict, char* key, void* val, size_t val_size){
    unsigned long num=hash(key)%dict->capacity;
    HashEntry* cur=dict->buckets[num];
    while(cur->next){
        cur=cur->next;
        if(strcmp(cur->key,key)==0){
            void* cpy=malloc(val_size);
            memcpy(cpy,val,val_size);
            free(cur->val);
            cur->val=cpy;
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

    dict->_size+=1;
    if(dict->_size>RESIZE_FACTOR*dict->capacity){
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

}