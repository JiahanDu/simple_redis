#include <stdio.h>

typedef struct Node{
    int val;
    Node* next;
    Node* lower;
    Node* upper;
}Node;

typedef struct Skiplist{
    Node* bottom;
    Node* top;
    int level;
}Skiplist;

Skiplist* create();

int search(SkipList* obj, int target);

void add(SkipList* obj, int num);

int remove(SkipList* obj, int num);

void skiplistFree(Skilist* obj);