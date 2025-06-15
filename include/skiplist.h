#include <stdbool.h>

typedef struct Node{
    int val;
    struct Node* next;
    struct Node* lower;
}Node;


typedef struct Skiplist{
    Node* top;
    int level;
} Skiplist;

Skiplist* skiplistCreate();

bool skiplistSearch(Skiplist* obj, int target);

void skiplistAdd(Skiplist* obj, int num);

bool skiplistErase(Skiplist* obj, int num);

void skiplistFree(Skiplist* obj);