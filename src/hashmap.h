#ifndef RESIZE_FACTOR
#define RESIZE_FACTOR 0.75
#endif

typedef struct HashEntry{
    char* key;
    void* val;
    struct HashEntry* next;
} HashEntry;

typedef struct HashMap{
    HashEntry** buckets;
    int capacity;
    int _size;
} HashMap;

int hash(char* key);
int size(HashMap* dict);
void add(HashMap* dict, char* key, void* val);
void remove(HashMap* dict, char* key);
void* get(HashMap* dict, char* key);
void resize(HashMap* dict);
void destroy(HashMap* dict);
