#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>

struct Node {
    int key;
    struct Node *parent
    struct Node *left;
    struct Node *right;
    int height;
};
typedef struct Node Node;

int height(Node *n){
    if(n == NULL){
        return -1;        //se estiver nulo
    }else{
        return n -> height; //caso contrário retornar nó apontando para a sua altura (?)
    }
}

Node * node_alloc(int key)
{
    Node *nd = (Node *)malloc(sizeof(Node));
    if (nd) {
        nd->key = key;
        nd->height = 0;
        nd->parent = NULL;
        nd->left = NULL;
        nd->right = NULL;
    }
    return nd;
}

void node_free(Node *nd)
{
    free(nd);
}

int main() {

    return 0;
}
