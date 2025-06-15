typedef struct Node{
    int val;
    struct Node* next;
    struct Node* lower;
}Node;


typedef struct Skiplist{
    Node* bottom;
    Node* top;
    int level;
} Skiplist;


Skiplist* skiplistCreate() {
    Node* head;
    head=malloc(sizeof(Node));
    head->val=-1;
    head->next=NULL;
    head->lower=NULL;
    Skiplist* res;
    res=malloc(sizeof(Skiplist));
    res->bottom=head;
    res->top=head;
    res->level=0;
    return res;
}

bool skiplistSearch(Skiplist* obj, int target) {
    Node* cur=obj->top;
    while(cur){
        if(cur->val==target){
            return true;
        }
        if(!cur->next || cur->next->val>target){
            cur=cur->lower;
        }else{
            cur=cur->next;
        }
    }
    return false;
}

void skiplistAdd(Skiplist* obj, int num) {
    int k=1;
    for(;;){
        if(rand()%2==0){
            k+=1;
        }else{
            break;
        }
    }
    if(k>obj->level){
        for(int i=0;i<k-obj->level;i++){
            Node* dummy=malloc(sizeof(Node));
            dummy->val=-1;
            dummy->next=NULL;
            dummy->lower=obj->top;
            obj->top=dummy;
        }
        obj->level=k;
    }
    Node* prev[obj->level];
    Node* cur=obj->top;
    for(int i=0;i<obj->level;i++){
        while(cur->next && cur->next->val<num){
            cur=cur->next;
        }
        prev[i]=cur;
    } 
    Node* temp[k];
    for(int i=obj->level-k;i<obj->level;i++){
        Node* node=malloc(sizeof(Node));
        node->val=num;
        node->next=NULL;
        node->lower=NULL;
        node->next=prev[i]->next;
        prev[i]->next=NULL;
        temp[i-(obj->level-k)]=node;
    }
    for(int i=0;i<k-1;i++){
        temp[i]->lower=temp[i+1];
    }
}

bool skiplistErase(Skiplist* obj, int num) {
    bool res=false;
    Node* cur=obj->top;
    for(int i=0;i<obj->level;i++){
        while(cur->next && cur->next<num){
            cur=cur->next;
        }
        if(cur->next && cur->next->val==num){
            cur->next=cur->next->next;
            res=true;
        }
        cur=cur->lower;
    }
    return res;
}

void help(Node* node){
    if(!node){
        return;
    }
    help(node->next);
    free(node);
}

void skiplistFree(Skiplist* obj) {
    Node* cur=obj->top;
    while(cur){
        Node* new_cur=cur->lower;
        free(cur);
        cur=new_cur;
    }
}