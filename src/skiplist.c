Skiplist* skiplistCreate() {
    Node* head;
    head=malloc(sizeof(Node));
    head->val=-1;
    head->next=NULL;
    head->upper=NULL;
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
    for(::){
        if(rand()%2==0){
            k+=1;
        }else{
            break;
        }
    }
    Node* prev[k];
    Node* temp[k];
    Node* cur=obj->top;
    for(int i=0;i<level;i++){
        while(cur->next && cur->next->val<num){
            cur=cur->next;
        }
        prev[i]=cur;
    }
    for(int i=0;i<level;i++){
        Node* node=malloce(sizeof(Node));
        node->val=num;
        Node->next=NULL;
        node->lower=NULL;
        node->upper=NULL;
        node->next=prev[i]->next;
        prev[i]->next=NULL;
        temp[level]=node;
    }
    for(int i=0;i<level;i++){
        if(i>0){
            temp[i]->upper=temp[i-1];
        }
        if(i<level-1){
            temp[i]->lower=temp[i+1];
        }
    }
    Node* cur=obj->bottom;
    for(int i=0;i<k-level;i++){
        while(cur){
            Node* p=malloc(sizeof(node));
            memcpy(p,cur,sizeof(node));
            cur=cur->next;
            p=p->next;
        }
        obj->bottom=obj->bottom->next;
        obj->level+=1;
    }
}

bool skiplistErase(Skiplist* obj, int num) {
    
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