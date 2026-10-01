#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>

struct Node {
    int key;
    struct Node *parent; 
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
    return T;
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
    Node *y = NULL; // y vai acompanhar o pai de onde inserir
    Node *x = T->root; //começa na raiz

    while (x != NULL) {
        y = x; // guarda o nó atual em y
        if (z->key < x->key) { //se a chave é menor desce na arvore pela esquerda
            x = x->left;
        } else { // se for maior desce pela direita
            x = x->right;
        }
    }

    z->parent = y;

    if (y == NULL) { // se no caso de não existir pai, z vira a raiz
        T->root = z;
    } else if (z->key < y->key) {
        y->left = z;
    } else {
        y->right = z;
    }
}

Node * bst_successor(Node *x) // para encontrar o nó com a menor chave maior que x
{
    if (x->right != NULL) { // se ele tiver um filho no lado direito o sucessor vai estar por lá
        return bst_minimum(x->right);
    }
    Node *y = x->parent; // se não tiver sobe e procura o sucessor a partir do nó pai
    while (y != NULL && x == y->right) {
        x = y;
        y = y->parent;
    }
    return y; // o loop vai terminar quando o y tiver o proximo elemento, se não, não há sucessor para x
}

Node * bst_predecessor(Node *x) // o maior valor menor que x, irá seguir a mesma lógica do sucessor só que ao contrário
{
    if (x->left != NULL) { 
        return bst_maximum(x->left);
    }
    Node *y = x->parent;
    while (y != NULL && x == y->left) {
        x = y;
        y = y->parent;
    }
    return y;
}

Node * bst_search(Node *x, int key) // irá procurar uma chave na arvore
{
    while (x != NULL && key != x->key) { // enquanto existe nó que não é a chave
        if (key < x->key) { // se chave for menor
            x = x->left;
        } else { // se a chave for maior
            x = x->right;
        }
    }
    return x;
}

void transplant(BST *T, Node *u, Node *v) // vai substituir uma subárvore de raiz u por outra de raiz v
{
    if (u->parent == NULL) { // se u for a raiz, v vai virar a nova raiz
        T->root = v;
    } else if (u == u->parent->left) { // se u for filho esquerdo seu pai aponta pra v
        u->parent->left = v; 
    } else {
        u->parent->right = v; // se u for o filho direito o pai aponta pra v
    }

    if (v != NULL) { // faz o pai de v apontar para o mesmo lugar
        v->parent = u->parent;
    }
}

void bst_delete(BST *T, Node *z) // vai remover um nó
{
    if (z->left == NULL) {
        transplant(T, z, z->right);
    } else if (z->right == NULL) {
        transplant(T, z, z->left);
    } else {
        Node *y = bst_minimum(z->right);

        if (y->parent != z) {
            transplant(T, y, y->right);
            y->right = z->right;
            y->right->parent = y;
        }

        transplant(T, z, y);
        y->left = z->left;
        y->left->parent = y;
    }

    node_free(z);
}

int bst_size(Node *x) // para saber o tamanho da arvore
{
    if (x == NULL) // se não existe retorna 0
        return 0;
    else // conta de forma recursiva o nó autal com o nó da direita e esquerda
        return 1 + bst_size(x->left) + bst_size(x->right);
}

void bst_printRec(Node *nd) // printa a arvore
{
    if (nd) {
        bst_printRec(nd->left);
        printf("%02d ", nd->key);
        bst_printRec(nd->right);
    }
}

void bst_print(BST *T) // printa a arvore
{
    printf("BST: [ ");
    bst_printRec(T->root);
    printf("]\n");
}

void bst_store(Node *nd, int *arr, int *index) // percorre a arvore e armazena em ordem em um vetor de forma recursiva
{
    if (nd) {
        bst_store(nd->left, arr, index); 
        arr[(*index)++] = nd->key; // vai obter o indice atual, armazenar a chave no vetor e se incrementar após (também não entendi) 
        bst_store(nd->right, arr, index);
    }
}

int int_comp(const void *a, const void *b) // não entendi pra que isso serve
{
    return (*(int *)a - *(int *)b);
}

bool bst_check(Node *nd, const int * const data, const int N) // verifica se as cahves da arvore são as mesma armazenadas em data
{
    const int tsize = bst_size(nd); // quantos nos tem

    if (tsize != N) { // se o quantidade é diferente nao sao iguais
        return false;
    }
    if (N == 0) { // se ambos forem vazios é igual
        return true;
    }

    int *arr = (int *)malloc(sizeof(int) * N); // copia o vetor em data para arr
    memcpy(arr, data, sizeof(int) * N);
    qsort(arr, N, sizeof(int), int_comp); // ordena arr

    int *tarr = (int *)malloc(sizeof(int) * tsize); // cria um vetor para a arvore
    int index = 0;
    bst_store(nd, tarr, &index); // colaca as chaves de tarr em ordem

    // for (int i = 0; i < N; i++)
    //    printf("(%02d ; %02d)\n", arr[i], tarr[i]);
    // fflush(stdout);

    bool match = true; // assumme que já estão ordenador e percorre comparando até achar um valor que não seja igual
    for (int i = 0; i < N && match; i++) {
        if (arr[i] != tarr[i]) {
            match = false;
        }
    }

    free(tarr); // libera os vetores
    free(arr);

    return match;
}

void bst_printTreeRec(Node *root, int space) // mas uma rotina para imprimir a arvore no terminal, não sei se é preciso explicar mesmo
{
    if (root != NULL) {
        space += 4;

        bst_printTreeRec(root->right, space);

        printf("\n");
        for (int i = 4; i < space; i++) {
            printf(" ");
        }
        printf("%02d\n", root->key);

        bst_printTreeRec(root->left, space);
    }
}

void bst_printTree(Node *root) // mesma coisa
{
    bst_printTreeRec(root, 0);
}


/*------------------------------------------------------------------------------
 * Permutação de um Arranjo
 *
 * Implementan o algoritmo iterativo de Narayana Pandita para a permutação de
 * um arranjo em ordem lexicográfica.
 */

void swap(int *a, int *b) // vai trocar dois valores, valor de a vai pr b e valor de b vai pra a
{
    const int temp = *a;
    *a = *b;
    *b = temp;
}

void perm_invert(int *arr, int inicio, int fim) // inverte um vetor em certas partes dele
{
    while (inicio < fim) {
        swap(&arr[inicio], &arr[fim]);
        inicio++;
        fim--;
    }
}

bool perm_next(int *arr, int tamanho) // essa rotina deve encontrar todas as permutações usando as duas rotinas anteriores para ajudar, agora u funcionamento do código não entendi direito
{
    int i = tamanho - 2;

    while (i >= 0 && arr[i] >= arr[i + 1]) {
        i--;
    }

    if (i < 0) {
        return false;
    }

    int j = tamanho - 1;
    while (arr[j] <= arr[i]) {
        j--;
    }

    swap(&arr[i], &arr[j]);

    perm_invert(arr, i + 1, tamanho - 1);

    return true;
}


/*------------------------------------------------------------------------------
 * Funções Auxiliares
 */

void data_print(const int * const data, const int N) // imprime os valores em data
{
    printf("data: [ ");
    for (int i = 0; i < N; i++) {
        printf("%02d ", data[i]);
    }
    printf("]\n");
}

int arr_remove(int *arr, int N, int value) // ajuda a remover certo valor do array
{
    for (int i = 0; i < N; i++) { // percorre o vetor até encontrar indice do mesmo valor e apaga ele
        if (arr[i] == value) {
            for (int j = i; j < N - 1; j++) {
                arr[j] = arr[j + 1];
            }
            N--;
            i--;
        }
    }

    return N;
}

int main() {

    int DATA_INSERT[] = {1, 2, 3, 4, 5}; // DEVE estar ordenado!
    int DATA_REMOVE[] = {1, 2, 3, 4, 5}; // DEVE estar ordenado!
    const int N = 5; // tamanho dos arranjos de inserção e remoção

    int *data_insert, *data_remove;

    data_insert = (int *)malloc(sizeof(int) * N);
    memcpy(data_insert, DATA_INSERT, sizeof(int) * N);
    
    do { // Loop de Inserção
    
        data_remove = (int *)malloc(sizeof(int) * N);
        memcpy(data_remove, DATA_REMOVE, sizeof(int) * N);

        do { // Loop de Remoção
            BST *T = bst_alloc();

            printf("--------------------------------------------\n");
            printf("Dados para Insercao:\n\t");
            data_print(data_insert, N);
            
            for (int i = 0; i < N; i++) {
                printf("Inserindo: %02d\n", data_insert[i]);
                Node *nd = node_alloc(data_insert[i]);
                bst_insert(T, nd);
                bst_print(T);
                assert(bst_check(T->root, data_insert, i + 1));
            }
            
            printf("Arvore apos todas as INSERCOES:\n");
            bst_printTree(T->root);

            printf("Dados para Remocao:\n\t");
            data_print(data_remove, N);
            int *arr = (int *)malloc(sizeof(int) * N);
            int asize = N;
            memcpy(arr, data_insert, sizeof(int) * asize);
            
            for (int i = 0; i < N; i++) {
                printf("Removendo: %02d\n", data_remove[i]);
                bst_delete(T, bst_search(T->root, data_remove[i]));
                bst_print(T);
                asize = arr_remove(arr, asize, data_remove[i]);
                assert(bst_check(T->root, arr, asize));
            }
            
            printf("Arvore apos todas as REMOCOES:\n");
            bst_printTree(T->root);

            free(arr);
            bst_free(T);

        } while (perm_next(data_remove, N));
        
        free(data_remove);

    } while (perm_next(data_insert, N));
    
    free(data_insert);

    return EXIT_SUCCESS;
}
