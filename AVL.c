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
        nd->height = 1; //uma folha
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

Node * minimum(Node* x){         //cria ponteiro auxiliar
    while(x -> left != NULL){    //laço de repetição que verifica nós a esquerda do que se verifica agora
        x = x -> left;           //se sim, move o ponteiro x pra esquerda
    }
return x;                        //while encerra, o ponteiro retornado é o de menor valor da subárvore
}

Node * maximum(Node *x){         //cria ponteiro auxiliar
    while (x -> right != NULL) { //verifica nós a direita do que se verifica agora
        x = x -> right;          //se sim, move ponteiro x para direita
    }
    return x;                    //while encerra, o ponteiro retornado é o maior da subárvore
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



int main() {

    return 0;
}
