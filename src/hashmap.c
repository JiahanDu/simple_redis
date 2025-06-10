#include "hashmap.h"

unsigned long hash(char* key){
    unsigned long hash=5381;
    int c;

    while((c=*key++)){
        hash=hash*33+c;
    }

    return hash;
}

size_t size(HashMap* dict){
    return dict->_size;
}