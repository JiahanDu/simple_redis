#ifndef RESIZE_FACTOR
#define RESIZE_FACTOR 0.75
#endif

typedef struct HashEntry{
    char* key;
    void* val;
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
void add(HashMap* dict, char* key, void* val, size_t val_size);
void remove(HashMap* dict, char* key);
void* get(HashMap* dict, char* key);
void resize(HashMap* dict);
void destroy(HashMap* dict);
