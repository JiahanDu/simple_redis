#include "hashmap.h"

int main(){
    HashMap* dict=initialize(10);
    for(int i=0;i<100;i++){
        char key_buffer[20];
        snprintf(key_buffer, sizeof(key_buffer),"test_key %d",i);
        char val_buffer[20];
        snprintf(val_buffer, sizeof(val_buffer),"test_val %d",i);
        add(dict,key_buffer,val_buffer,STRING,strlen(val_buffer)+1);
        printf("%d th iteration\n", i);
        printf("capacity is %zu\n", dict->capacity);
    }

    for(int i=0;i<100;i++){
        char key_buffer[20];
        snprintf(key_buffer, sizeof(key_buffer),"test_key %d",i);
        HashEntry* entry=get(dict,key_buffer);
        printf("%d th query, value is %s\n", i, (char *)entry->val);
    }

    for(int i=0;i<100;i++){
        char key_buffer[20];
        snprintf(key_buffer, sizeof(key_buffer),"test_key %d",i);
        pop(dict,key_buffer);
        printf("After %d th pop, size is %zu.\n", i, dict->size);
    }

    for(int i=0;i<100;i++){
        char val_buffer[20];
        snprintf(val_buffer, sizeof(val_buffer),"test_val %d\n",i);
        add(dict,"apple",val_buffer,STRING, strlen(val_buffer)+1);
        printf("After %d th loop, value of apple is %s", i, (char*)get(dict,"apple")->val);
    }

    for(int i=0;i<100;i++){
        HashEntry* entry=get(dict,"applepie");
        if(!entry){
            printf("dict does not contain applepie\n");
        }
    }

    HashMap* dict2=initialize(20);
    add(dict, "apple", dict2, HASH, sizeof(dict2));
    printf("Size of dict is %zu.\n",dict->size);
    printf("Type of value corresponding to key apple is %d\n",get(dict,"apple")->type);
    destroy_map(dict);
    return 0;
}