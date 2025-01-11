/*
    This is a splay tree program where we can create a normal splay tree and also a modified splay tree which works based on
how many times a need have been reached(that is frequency of the node).
    Below are the brief list of functions implemented:
structure
newNode
Lrotation
Rrotation
splay
insert
    -find (they both work nearly the same)
delete
pre-order traversal

Arda Can Ergin (id: 150123060), Yusuf Çetin (id: 150122003)
*/

#include <stdlib.h>
#include <stdio.h>

/*Node structure*/
typedef struct Node
{
    int key;
    int freq;
    struct Node* left;
    struct Node* right;
    struct Node* parent;
}Node;

/*importtant global variables to keep track of cost*/
int rotCount=0;
int compCount=0;
int topFreq=0;

/*variables that will help to appropriately splay a tree*/
Node* topFreqNode=NULL;
Node* oldTopFreqNode=NULL;

/*new node method to create a new node*/
Node* newNode(int key){
    Node* node = (Node*)malloc(sizeof(Node));
    if(node==NULL)
        printf("newNode key: %d error!",key);
    else 
    {
        node->key=key;
        node->left=NULL;
        node->right=NULL;
        node->parent=NULL;
        node->freq=0;
    }

    return node;
}

/*zig (lright rotation)*/
Node* Rrotation(Node* alpha){

    if(alpha==NULL || alpha->left==NULL) return alpha;

    /*new root is the left of the old root (alpha)*/
    Node* newRoot= alpha->left;

    /*update the left child of old root*/
    alpha->left=newRoot->right;
        if(newRoot->right!=NULL) newRoot->right->parent = alpha;
    newRoot->right=alpha;

    /*update parent*/
    newRoot->parent=alpha->parent;
    alpha->parent=newRoot;

    /*update rotation*/
    rotCount++;
    return newRoot;
}

/*zig (left rotation)*/
Node* Lrotation(Node* alpha){

    if(alpha==NULL || alpha->right==NULL) return alpha;

    Node* newRoot= alpha->right;

    alpha->right=newRoot->left;
        if(newRoot->left!=NULL) newRoot->left->parent = alpha;
    newRoot->left=alpha;

    newRoot->parent=alpha->parent;
    alpha->parent=newRoot;

    rotCount++;
    return newRoot;
}

Node* splay(Node* root,int key){

    if(root ==NULL || root->key==key)
        return root;

    /*we will make a final rotation to fix */
    if(key<root->key)
    {
        /*key is not in the tree*/
        if(root->left ==NULL) return root;

        /*zig-zig double rotation same direction(right-right)*/
        if(key<root->left->key){

            root->left->left = splay(root->left->left,key);
            root= Rrotation(root); /*we can make the rotation on root->left to reduce the height of the tree(does not always work)*/
        }
        /*zig-zag double rotation opposite direction(left-right)*/
        else if(key>root->left->key){

            root->left->right= splay(root->left->right,key);
            if(root->left->right!=NULL)
            root->left = Lrotation(root->left);

        }

        /*final rotation to fix initial condiition*/
        if(root->left==NULL) /*return to grandparent*/
        return root;         /*this implementation allows us to rotate for root's child recursively*/
        else
        return root=Rrotation(root);
    }

    else if(key>root->key){

        /*key is not in the tree*/
        if(root->right ==NULL) return root;

        /*zig-zig double left rotation*/
        if(key>root->right->key){

            root->right->right = splay(root->right->right,key);
            root=Lrotation(root); /*we can make the rotation on root->right to reduce tree height ( does not always work)*/

        }
        
        /*zig-zag double rotation opposite directions right-left*/
        else if(key<root->right->key){

            root->right->left = splay(root->right->left,key);
            if(root->right->left!=NULL)
            root->right = Rrotation(root->right);

        }

        /*final rotation*/
        if(root->right==NULL) /*return to parent*/
        return root;
        else 
        return root=Lrotation(root); /*handle the child play*/
        
    }
}


Node* insert(Node* root, int key){
    /*i dont need to update compCount since it splays the tree at the start*/
    
    if(root ==NULL || root->key == key) return root;

    /*splay with non-existing key so that node closer to key will be in the root*/
    root = splay(root,key);

    if(root->key==key){
        return root;
    }

    /*create new node*/
    Node* node = newNode(key);

    /*append key as the new root to correct position*/
    if(key>root->key){
        compCount++;
        if(root->right!=NULL && key<root->right->key){
            node->left=root;
            node->right=root->right;
            root->right=NULL;
            root=node;
        }else
        {
        node->left=root;
        root=node;
        }
    }
    else if(key<root->key)
    {   
        compCount++;
        if(root->left!=NULL && key>root->left->key)
        {
            node->right=root;
            node->left=root->left;
            root->left=NULL;
            root=node;
        }
        else {

        node->right=root;
        root=node;

        }
    }

    return root;
}

Node* find (Node* root, int key){

    if(root==NULL) return root; /*key not found*/

    if(key>root->key){
        
       find(root->right,key);

    }
    else if(key<root->key){
        
        find(root->left,key);
    }
    else { /*if key alread exists in the tree increase frequency*/
        root->freq++;
        if(topFreq<root->freq){
            topFreq=root->freq;
            topFreqNode=root;

        }

        /*if top freq node is updated meaning that our root is now topfreqNode splay the tree*/
        if(oldTopFreqNode!=topFreqNode){
        root=splay(oldTopFreqNode,topFreqNode->key); /* bring topFreqNode to the root using oldTopFreqNode as the root*/
        oldTopFreqNode=topFreqNode; /*update oldTopFreqNode*/
    }
    return root;
    }

}


Node* modInsert(Node *root, int key){

    if(root==NULL) /*key does not exists*/
    {
        Node* node = newNode(key);
        root=node;
        return root;
    }

    if(key>root->key){
        compCount++;
        root->right=modInsert(root->right,key);
    }
    else if(key<root->key){
        compCount++;
        root->left=modInsert(root->left,key);
    }
    else { /*if key alread exists in the tree increase frequency, work like a search function*/
        compCount++;
        root->freq++;
        if(topFreq<root->freq){
            topFreq=root->freq;
            topFreqNode=root;

        }
    }

    
    if(oldTopFreqNode!=topFreqNode && root==oldTopFreqNode){
        root=splay(oldTopFreqNode,topFreqNode->key); /* bring topFreqNode to the root using oldTopFreqNode as the root*/
        oldTopFreqNode=topFreqNode; /*update oldTopFreqNode*/
    }
    return root;
}


Node* delete(Node *root, int key){
    
    if(root->key==key || root==NULL) return root;

    /*bring the key to the root for delete*/
    root= splay(root,key);

    if(root->key!=key) return root; /*key is not in the tree deletion is not possible*/

    /*reason for checking if the right or left is null is to make program faster,getting rid of redundant splay calls*/
    if(root->right==NULL){

        Node* oldRoot = root;
        root=root->left;

        free(oldRoot);
    }
    else if(root->left==NULL)
    {
        Node* oldRoot = root;
        root=root->right;

        free(oldRoot);
    }
    else /*consider splayed tree is perfect*/
    {
        Node* oldRoot=root;

        root=splay(root->left,key);
        root->right=oldRoot->right;

        free(oldRoot);
    }

    return root;
}

/*pre-order traversal*/
void preOrder(Node* node){
    if(node!=NULL)
    {
        printf(" %d, ", node->key);
        preOrder(node->left);
        preOrder(node->right);
    }
}

void preOrderWfreq(Node* node){
    if(node!=NULL)
    {
        printf(" %d(%d), ", node->key,node->freq);
        preOrderWfreq(node->left);
        preOrderWfreq(node->right);
    }
}
/*pre-order traversal for printing to output file*/
void writePreOrderFreq(FILE* out,Node* node){
    if(node!=NULL)
    {
        fprintf(out," (%d,%d), ", node->key,node->freq);
        writePreOrderFreq(out,node->left);
        writePreOrderFreq(out,node->right);
    }
}

void writePreOrder(FILE* out,Node* node){
    if(node!=NULL)
    {
        fprintf(out,"%d, ", node->key);
        writePreOrder(out,node->left);
        writePreOrder(out,node->right);
    }
}

int main(){

    /*read from the file*/

    FILE* input;
    FILE* out;
    int key;

    input=fopen("/home/ardacan/my_c_project/project_2/input.txt","r"); 

    /*error check*/
    if(input==NULL){
        puts("file open error!");
        return -1;
    }

    out=fopen("output.txt","w");

    if(out==NULL){
        puts("file open error: output.txt!");
        return -2;
    }

    fscanf(input,"%d,", &key); /*read the first node separately, initialize the tree*/
    Node* rootSplay = newNode(key); /* no frequency update needed*/ oldTopFreqNode=rootSplay;

    while (fscanf(input,"%d,", &key)!=EOF)
    {
        rootSplay=insert(rootSplay,key); 
    }

    /*write to the console*/
    preOrder(rootSplay); printf("\n Cost: %d ",rotCount+compCount); puts(""); puts("-----------------------------------------------------------");

    /*write to the output file*/
    fprintf(out,"Splay Tree \nTree (pre-order traversal): ");
    writePreOrder(out,rootSplay); fprintf(out,"\n");
    fprintf(out,"Cost: %d\n", rotCount+compCount);
    fprintf(out,"----------------------------------------------------------------------------\n");

    /*initialize rotation count and comparision count*/
    rotCount=0; compCount=0; oldTopFreqNode=NULL;

    /* restart file pointer from the begining of the file*/
    rewind(input); 

    fscanf(input, "%d,",&key); /*always read the first number separately, so we can use it as a root to start the tree*/
    Node* rootMod= newNode(key); oldTopFreqNode=rootMod; topFreqNode=rootMod;

    /*after initializing the tree start adding other nodes*/
    while(fscanf(input, "%d,",&key)!=EOF)
    {
        rootMod=modInsert(rootMod,key);
    }

    /*write to the console*/
    preOrderWfreq(rootMod); printf("\n Cost: %d ",rotCount+compCount); puts(" ");

    /*write to the output file*/
    fprintf(out,"Mod-Splay Tree \nTree (pre-order traversal): ");
    writePreOrderFreq(out,rootMod); fprintf(out,"\n");
    fprintf(out,"Cost: %d\n", rotCount+compCount);

    fclose(out);
    fclose(input);
    
}   