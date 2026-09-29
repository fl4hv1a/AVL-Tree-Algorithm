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

struct BinarySearchTree // nó da árvore
{
    struct Node *root;
};
typedef struct BinarySearchTree BST;

BST * bst_alloc() // aloca memória para a estrutura da árvore , cria ela vazia e retorna
{
    BST *T = (BST *)malloc(sizeof(BST));
    if (T) {
        T->root = NULL;
    }
    return t;
}

void bst_freeRec(Node *nd) // libera a árvore de forma recursiva começando pelo nó filho até o nó pai
{
    if (nd) {
        bst_freeRec(nd->left);
        bst_free_Rec(nd->right);
        node_free(nd);
    }
}

void bst_free(BST * T) 
{
    bst_freeRec(T->root);
    free(T); // libera a própria estrutura bst
}

void bst_insert(BST *T, Node *z)
{
    Node *y = NULL;
    Node *x = T->root;

    while (x != NULL) {
        y = x;
        if (z->key < x->key) {
            x = x->left;
        } else {
            x = x->right;
        }
    }

    z->parent = y;

    if (y == NULL) {
        T->root = z;
    } else if (z->key < y->key) {
        y->left = z;
    } else {
        y->right = z;
    }
}

Node * bst_minimum(Node *x) // encontra o menor elemento que está mais à esquerda
{
    while (x->left != NULL) {
        x = x->left;
    }
    return x;
}

Node * bst_maximum(Node *x) // encontra o maior elemento que está mais à direita
{
    while (x->right != NULL) {
        x = x->right;
    }
    return x;
}



int main() {

    return 0;
}
