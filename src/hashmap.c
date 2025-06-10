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
            cur->val=memcpy()
            cur->val_size=val_size;
            break;
        }
    }
    HashEntry* node=malloc(sizeof(HashEntry));
    if(!node){
        fprintf(stderr,"malloc() failed. %s, error code %d.\n", strerror(errno),errno);
        exit(errno);
    }
    node->key=strcpy(key);
    node->val=strcpy(val);
    node->val_size=val_size;
    cur->next=node;

    dict->_size+=1;
    if(dict->_size>RESIZE_FACTOR*dict->capacity){
        resize(dict);
    }
}