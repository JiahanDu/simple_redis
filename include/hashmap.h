#include <stdbool.h> 
#include <stddef.h>    
#include <stdio.h>    
#include <stdlib.h>    
#include <string.h>    
#include <errno.h>    

#ifndef RESIZE_FACTOR
#define RESIZE_FACTOR 0.75
#endif

typedef enum Type{
    STRING,
    HASH,
    DUMMY=-1
}Type;

typedef struct HashEntry{
    char* key;
    void* val;
    Type type;
    size_t val_size;
    struct HashEntry* next;
} HashEntry;

typedef struct HashMap{
    HashEntry** buckets;
    size_t capacity;
    size_t size;
} HashMap;

unsigned long hash(char* key);
HashMap* initialize(size_t capacity);
size_t size(HashMap* dict);
void add(HashMap* dict, char* key, void* val, Type type, size_t val_size);
bool pop(HashMap* dict, char* key);
HashEntry* get(HashMap* dict, char* key);
void resize(HashMap* dict);
void destroy_entry(HashEntry* entry);
void destroy_map(HashMap* dict);
