#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>

#define ALLOWED_IMBALANCE 1 // O desbalanceamento máximo permitido em uma AVL
#define TAB_SIZE 4 // Quantos espaços devem ser utilizados para representar um "TAB" no terminal

typedef struct Node {
    int key;
    struct Node *parent;
    struct Node *left;
    struct Node *right;
    int height; // Distância até o nó folha mais profundo entre as subárvores esquerda e direita
} Node;

typedef struct BinarySearchTree {
    Node *root;
} BST;

/**
 * @brief Encontra o maior entre dois inteiros.
 * 
 * @param[in] a O primeiro inteiro a ser comparado.
 * @param[in] b O segundo inteiro a ser comparado.
 * 
 * @retval - `a`: se `a > b`;
 * @retval - `b`: se `a <= b`.
 */
int max(const int a, const int b) {
    return (a > b) ? a : b;
}

/**
 * @brief Retorna a altura de um nó (distância até o nó folha mais profundo entre as subárvores filhas).
 * 
 * @details Retorna diretamente a altura do nó se ele for não-nulo e -1 caso contrário.
 *          Retornar -1 em nós nulos torna o cálculo do balanceamento mais simples
 *          para nós folha.
 * 
 * @param[in] n Ponteiro para o nó do qual se deseja receber a altura.
 * 
 * @return Altura do nó `n`.
 */
int height(Node *n) {
    if(n) {
        return n->height;
    } else {
        // Se 'n' for nulo, retornamos -1 para facilitar o cálculo da altura de nós folha.
        return -1;
    }
}

/**
 * @brief Recalcula e atualiza a altura de um nó.
 * 
 * @param[in] n Ponteiro para o nó que será atualizado.
 */
void update_height(Node *n) {
    if (n) {
        // Se 'n' for folha, a expressão se torna "1 + (-1) = 0"
        // (já que a altura de todo nó nulo é -1 por definição)
        n->height = 1 + max(height(n->left), height(n->right));
    }
}

/**
 * @brief Calcula o fator de balanceamento (FB) de um nó.
 * 
 * @details Retorna a diferença entre as alturas das subárvores filhas, ou `0`
 *          caso o nó seja nulo (para facilitar o cálculo do FB de nós folha).
 * 
 * @param[in] n Ponteiro para o nó do qual será calculado o balanceamento.
 * 
 * @return O fator de balanceamento do nó `n`.
 * 
 * @retval - Um valor `negativo`: Se a altura da subárvore direita for maior que a esquerda;
 * @retval - `0`: Se as duas subárvores tiverem a mesma altura;
 * @retval - Um valor `positivo`: Se a altura da subárvore esquerda for maior que a direita.
 */
int get_balance(Node *n) {
    if (!n) return 0; // Se o nó for nulo, já está "balanceado"
    return height(n->left) - height(n->right);
}

/**
 * @brief Substitui uma subárvore BST de raíz `u` por uma subárvore BST de raíz `v`.
 * 
 * @param[in] T Ponteiro para a árvore BST onde as subárvores estão contidas.
 * @param[in] u Ponteiro para o nó raíz da subárvore BST original.
 * @param[in] v Ponteiro para o novo nó raíz da subárvore BST.
 */
void transplant(BST *T, Node *u, Node *v) {
    if (u == T->root) {
        // Se `u` era a raiz, `v` vai virar a nova raiz
        T->root = v;
    } else if (u == u->parent->left) {
        // Se `u` era filho à esquerda, seu pai agora aponta para `v`
        u->parent->left = v;
    } else {
        // Se `u` era filho à direita, seu pai agora aponta pra `v`
        u->parent->right = v;
    }

    if (v) {
        // Se `v` for não-nulo, definimos o novo pai de `v`
        v->parent = u->parent;
    }
}

/**
 * @brief Realiza uma rotação à direita em um nó de uma árvore AVL.
 * 
 * @param[in] T Ponteiro para a árvore AVL onde será realizada a rotação.
 * @param[in] n Ponteiro para o nó que será rotacionado.
 */
void right_rotate(BST *T, Node *n) {
    Node *leftN = n->left; // Guarda o filho à esquerda de `n`

    transplant(T, n, leftN); // Insere o filho à esquerda de `n` no lugar do próprio `n`

    Node *RleftN = leftN->right; // Guarda o filho à direita do filho à esquerda de `n`
    
    // Transfere a subárvore direita de `leftN` para a subárvore esquerda de `n`
    n->left = RleftN;
    leftN->right = n;

    // Após a rotação, `n` se tornou filho de `leftN`
    n->parent = leftN;
    
    // Se a subárvore `RleftN` existir, ela mudou de lado e seu novo pai é `n`
    if (RleftN) {
        RleftN->parent = n;
    }

    // Atualiza as alturas
    update_height(n);
    update_height(leftN);
}

/**
 * @brief Realiza uma rotação à esquerda em um nó de uma árvore AVL.
 * 
 * @param[in] T Ponteiro para a árvore AVL onde será realizada a rotação.
 * @param[in] n Ponteiro para o nó que será rotacionado.
 */
void left_rotate(BST *T, Node *n) {
    Node *rightN = n->right; // Guarda o filho à direita de `n`

    transplant(T, n, rightN); // Insere o filho à direita de `n` no lugar do próprio `n`

    Node *LrightN = rightN->left; // Guarda o filho à esquerda do filho à direita de `n`

    // Transfere a subárvore direita de `leftN` para a subárvore esquerda de `n`
    n->right = LrightN;
    rightN->left = n;

    // Após a rotação, `n` se tornou filho de `rightN`
    n->parent = rightN;
    
    // Se a subárvore `LrightN` existir, ela mudou de lado e seu novo pai é `n`
    if (LrightN) {
        LrightN->parent = n;
    }

    // Atualiza as alturas
    update_height(n);
    update_height(rightN);
}

/**
 * @brief Rebalanceia, por meio de rotações, um nó de uma árvore AVL.
 * 
 * @param[in] T Ponteiro para a árvore AVL onde será realizado o rebalanceamento.
 * @param[in] n Ponteiro para o nó que será rebalanceado.
 */
void avl_node_rebalance(BST *T, Node *n) {
    int balance = get_balance(n);
    if (balance > ALLOWED_IMBALANCE) {
        if (get_balance(n->left) < 0) { 
            left_rotate(T, n->left); // Metade da rotação dupla à direita
        }

        right_rotate(T, n); // Rotação simples à direita
    } else if (balance < -ALLOWED_IMBALANCE) {
        if (get_balance(n->right) > 0) {
            right_rotate(T, n->right); // Metade da rotação dupla à esquerda
        }

        left_rotate(T, n); // Rotação simples à esquerda
    }
    // Se `-ALLOWED_IMBALANCE < balance < ALLOWED_IMBALANCE`:
    //      `n` já está balanceado, então não fazemos nada
}

/**
 * @brief Rebalanceia uma árvore AVL após uma modificação estrutural.
 * 
 * @param[in] T Ponteiro para a árvore AVL onde acontecerá o rebalanceamento.
 * @param[in] n Ponteiro para o nó mais profundo onde o balanceamento pode ter sido alterado.
 */
void avl_balance(BST *T, Node *n) {
    while (n) {
        update_height(n); // Atualiza a altura do nó atual
        
        avl_node_rebalance(T, n); // Rebalanceia o nó atual
        
        n = n->parent; // Sobe para o próximo nó ancestral
    }
}

/**
 * @brief Aloca um nó na memória heap e preenche com a chave fornecida.
 * 
 * @param[in] key Chave a ser inserida no nó alocado.
 * 
 * @return Ponteiro para o nó alocado, ou NULL em caso de falha na alocação.
 */
Node *node_alloc(int key) {
    Node *n = (Node *) calloc(1, sizeof(Node));

    if (n) {
        n->key = key;
    }

    return n;
}

/**
 * @brief Desaloca um nó previamente alocado com node_alloc().
 * 
 * @param[in] n Ponteiro para o nó a ser desalocado.
 */
void node_free(Node *n) {
    free(n);
}

/**
 * @brief Busca e retorna o menor nó encontrado em uma subárvore BST.
 * 
 * @param[in] x Ponteiro não-nulo para a raíz da subárvore alvo.
 * 
 * @return Ponteiro para o menor nó encontrado na subárvore alvo.
 */
Node *bst_minimum(Node* x) {
    Node *y = NULL; // Ponteiro que armazenará a posição anterior de x

    while (x) { // Enquanto x for não-nulo
        y = x; // Armazena o endereço atual de x
        x = x->left; // Desce o ponteiro x pela esquerda
    }

    return y;
}

/**
 * @brief Busca e retorna o maior nó encontrado em uma subárvore BST.
 * 
 * @param[in] x Ponteiro não-nulo para a raíz da subárvore alvo.
 * 
 * @return Ponteiro para o maior nó encontrado na subárvore alvo.
 */
Node *bst_maximum(Node *x) {         
    Node *y = NULL; // Ponteiro que armazenará a posição anterior de x

    while (x) { // Enquanto x for não-nulo
        y = x; // Armazena o endereço atual de x
        x = x->right; // Desce o ponteiro x pela direita
    }

    return y;
}

/**
 * @brief Aloca uma BST vazia na memória heap.
 * 
 * @return Ponteiro para a BST alocada, ou NULL em caso de falha na alocação.
 */
BST *bst_alloc() {
    return (BST *) calloc(1, sizeof(BST));
}

/**
 * @brief Desaloca recursivamente (das folhas até a raíz) uma subárvore composta por nós previamente alocados com node_alloc().
 * 
 * @param[in] n Ponteiro para o nó raíz da subárvore a ser desalocada.
 */
void bst_freeRec(Node *n) {
    if (n) {
        bst_freeRec(n->left);
        bst_freeRec(n->right);
        node_free(n);
    }
}

/**
 * @brief Desaloca uma árvore BST (e todos os seus nós) previamente alocada com bst_alloc().
 * 
 * @param[in] T Ponteiro para a árvore BST a ser desalocada.
 */
void bst_free(BST *T) {
    bst_freeRec(T->root); // Desaloca recursivamente todos os nós

    free(T); // Desaloca a própria estrutura bst
}

/**
 * @brief Insere um novo nó em uma árvore BST.
 * 
 * @param[in] T Ponteiro para a árvore BST onde o nó deve ser inserido.
 * @param[in] z Ponteiro para o nó a ser inserido.
 */
void bst_insert(BST *T, Node *z) {
    Node *y = NULL; // `y` vai acompanhar o pai de onde inserir
    Node *x = T->root; // `x` vai rastrear a posição atual durante a busca na árvore

    while (x) {
        y = x; // Guarda o nó atual em `y`
        if (z->key < x->key) {
            // Se a chave é menor, descemos pela subárvore esquerda
            x = x->left;
        } else {
            // Se a chave é maior, descemos pela subárvore direita
            x = x->right;
        }
    }

    z->parent = y; // Define o pai de `z` como `y`

    if (!y) {
        // Se o pai de `z` for nulo, `T` está vazia, então `z` vira a raiz
        T->root = z;
    } else if (z->key < y->key) {
        // Se `z` tiver uma chave menor que a do seu pai, inserimos `z` à esquerda
        y->left = z;
    } else {
        // Se `z` tiver uma chave maior que a do seu pai, inserimos `z` à direita
        y->right = z;
    }
}

/**
 * @brief Insere um novo nó em uma árvore AVL e refaz o balanceamento.
 * 
 * @param[in] T Ponteiro para a árvore AVL onde o nó deve ser inserido.
 * @param[in] z Ponteiro para o nó a ser inserido.
 */
void avl_insert(BST *T, Node *z) {
    // Começa fazendo uma inserção BST comum
    bst_insert(T, z);

    // Começa a rebalancear a partir do nó inserido subindo até a raiz
    avl_balance(T, z);
}

/**
 * @brief Busca e retorna o nó com a menor chave maior que a chave de x (o sucessor em-ordem de x)
 * 
 * @param[in] x Ponteiro para o nó predecessor em-ordem do nó que será buscado.
 * 
 * @return Ponteiro para o nó sucessor em-ordem de x, ou NULL caso nenhum sucessor seja encontrado.
 */
Node *bst_successor(Node *x) {
    if (x->right) { // Se ele tiver um filho à direita, o sucessor vai estar lá
        return bst_minimum(x->right);
    }

    // Se não tinha filho à direita, sobe e procura o sucessor a partir dos nós ancestrais
    Node *y = x->parent;
    while (y && x == y->right) { // Enquanto `y` for não-nulo e `x` for filho à direita de `y`
        x = y;
        y = y->parent;
    }

    return y;
}

/**
 * @brief Busca e retorna o nó com a maior chave menor que a chave de x (o predecessor em-ordem de x)
 * 
 * @param[in] x Ponteiro para o nó sucessor em-ordem do nó que será buscado.
 * 
 * @return Ponteiro para o nó predecessor em-ordem de x, ou NULL caso nenhum predecessor seja encontrado.
 */
Node *bst_predecessor(Node *x) {
    if (x->left) { 
        return bst_maximum(x->left);
    }

    Node *y = x->parent;
    while (y && x == y->left) {
        x = y;
        y = y->parent;
    }

    return y;
}

/**
 * @brief Busca por um nó em uma subárvore BST que contenha uma chave específica.
 * 
 * @param[in] x Ponteiro para o nó raíz da subárvore BST onde a busca será realizada.
 * @param[in] key Chave do nó que será buscado.
 * 
 * @return Ponteiro para o nó encontrado, ou NULL caso nenhum nó contendo a chave especificada seja encontrado.
 */
Node *bst_search(Node *x, int key) {
    while (x && key != x->key) { // Enquanto `x` for não-nulo e não tiver a chave desejada
        if (key < x->key) {
            // Se a chave desejada for menor, desce pela subárvore esquerda
            x = x->left;
        } else {
            // Se a chave desejada for maior, desce pela subárvore direita
            x = x->right;
        }
    }

    return x;
}

/**
 * @brief Remove e desaloca um nó de uma árvore BST.
 * 
 * @param[in] T Ponteiro para a árvore BST onde o nó será removido.
 * @param[in] z Ponteiro para o nó que será removido.
 * 
 * @return Ponteiro para o nó mais profundo da árvore onde o balanceamento
 *         pode ter sido quebrado (caso `T` seja uma AVL).
 */
Node *bst_delete(BST *T, Node *z) {
    Node *fix_node_pin = z->parent; // Serve para rastrear onde o balanceamento possivelmente mudará (caso `T` seja uma AVL)
    
    if (z->left == NULL) {
        // Não tem subárvore à esquerda, então colocamos a subárvore direta no lugar de `z`
        transplant(T, z, z->right);
    } else if (z->right == NULL) {
        // Não tem subárvore à direita, então colocamos a subárvore esquerda no lugar de `z`
        transplant(T, z, z->left);
    } else { // Tem duas subárvores, então colocaremos o sucessor em-ordem de `z` no lugar dele
        Node *y = bst_successor(z); // Menor elemento da subárvore direita de `z`

        if (y->parent != z) {
            fix_node_pin = y->parent; // Se o sucessor não é filho direto, o rebalanceamento começa no pai do sucessor
            transplant(T, y, y->right);
            y->right = z->right;
            y->right->parent = y;
        } else {
            fix_node_pin = y; // Se o sucessor era filho direto, o rebalanceamento começa no sucessor
        }

        transplant(T, z, y);
        y->left = z->left;
        y->left->parent = y;
    }

    node_free(z);

    return fix_node_pin;
}

/**
 * @brief Remove, rebalanceia e desaloca um nó de uma árvore AVL.
 * 
 * @param[in] T Ponteiro para a árvore AVL onde o nó será removido.
 * @param[in] z Ponteiro para o nó que será removido.
 */
void avl_delete(BST *T, Node *z) {
    // Primeiro faz uma remoção BST simples
    Node *fix_node_pin = bst_delete(T, z);

    // Depois começa o rebalanceamento subindo até chegar na raiz
    avl_balance(T, fix_node_pin);
}

/**
 * @brief Calcula recursivamente o tamanho (quantidade de nós) de uma subárvore BST.
 * 
 * @param[in] x Ponteiro para a raíz da subárvore BST a ser calculada.
 * 
 * @return Inteiro positivo representando a quantidade de nós existentes na subárvore BST fornecida.
 */
int bst_size(Node *x) {
    if (!x) // Se `x` for nulo, a subárvore está vazia
        return 0;
    else // Conta recursivamente o tamanho atual com o da direita e da esquerda
        return 1 + bst_size(x->left) + bst_size(x->right);
}

/**
 * @brief Imprime recursivamente uma representação textual de uma subárvore BST.
 * 
 * @param[in] n Nó raiz da subárvore a ser impressa.
 */
void bst_printRec(Node *n) {
    if (n) {
        bst_printRec(n->left);
        printf("%02d ", n->key);
        bst_printRec(n->right);
    }
}

/**
 * @brief Imprime recursivamente uma representação textual de uma árvore BST.
 * 
 * @param[in] T Ponteiro para a BST a ser impressa.
 */
void bst_print(BST *T) {
    printf("BST: [ ");
    bst_printRec(T->root);
    printf("]\n");
}

/**
 * @brief Percorre uma subárvore BST em-ordem e armazena suas chaves em um vetor.
 * 
 * @param[in] nd Ponteiro para o nó raiz da subárvore a ser percorrida.
 * @param[out] arr Ponteiro para um vetor (já alocado) onde as chaves serão armazenadas.
 * 
 * @param[in,out] index Ponteiro para a variável inteira que controla a posição atual no vetor. 
 *                      Deve apontar para um valor inicial de `0` na primeira chamada.
 */
void bst_store(Node *nd, int *arr, int *index) {
    if (nd) {
        bst_store(nd->left, arr, index); 
        arr[(*index)++] = nd->key; // Armazena a chave do nó atual e incrementa o índice "compartilhado" do arranjo
        bst_store(nd->right, arr, index);
    }
}

/**
 * @brief Função de comparação para ordenação crescente de inteiros.
 * 
 * @details Converte os ponteiros genéricos (void *) para ponteiros de inteiros (int *) e 
 *          subtrai os valores apontados para determinar a ordem relativa entre eles.
 * 
 * @param[in] a Ponteiro genérico para o primeiro elemento a ser comparado.
 * @param[in] b Ponteiro genérico para o segundo elemento a ser comparado.
 * 
 * @return O resultado da comparação.
 * 
 * @retval - Um inteiro `negativo`: Se `a` for menor que `b`;
 * @retval - `0`: Se `a` for igual a `b`;
 * @retval - Um inteiro `positivo`: Se `a` for maior que `b`.
 */
int int_comp(const void *a, const void *b) {
    return (*(int *)a - *(int *)b);
}

/**
 * @brief Verifica se uma subárvore BST possui exatamente as mesmas chaves
 *        (considerando-se um percurso em-ordem) de um vetor de referência.
 * 
 * @details A validação se inicia comparando o tamanho da árvore com o do vetor.
 *          Após isso, o vetor de referência é copiado e ordenado.
 *          Por fim, extrai as chaves da árvore (também em-ordem)
 *          e realiza uma comparação elemento a elemento.
 * 
 * @param[in] nd Ponteiro para o nó raiz da subárvore BST a ser verificada.
 * @param[in] data Ponteiro constante para um vetor contendo as chaves de referência.
 * @param[in] N Quantidade de elementos contidos no vetor de referência.
 * 
 * @return Resultado da validação da subárvore.
 * 
 * @retval - `true`: se a subárvore contiver exatamente as mesmas chaves em-ordem do vetor;
 * @retval - `false`: caso o tamanho da subárvore seja diferente ou alguma
 *           chave não corresponda ao seu respectivo valor de referência.
 */
bool bst_check(Node *nd, const int *const data, const int N) {
    const int tsize = bst_size(nd); // Quantos nós a subárvore tem

    if (tsize != N) { // Se a quantidade é diferente, não podem ser iguais
        return false;
    }
    if (N == 0) { // Se ambos forem vazios, são iguais
        return true;
    }

    int *arr = (int *) malloc(sizeof(int) * N);
    memcpy(arr, data, sizeof(int) * N); // Cria uma cópia do arranjo `data` no arranjo `arr`
    qsort(arr, N, sizeof(int), int_comp); // Ordena `arr`

    int *tarr = (int *) malloc(sizeof(int) * tsize); // Arranjo para as chaves da árvore 
    int index = 0; // `bst_store()` requer um índice inicial `0`
    bst_store(nd, tarr, &index); // Armazena as chaves em-ordem da subárvore em `tarr`

    // for (int i = 0; i < N; i++)
    //    printf("(%02d ; %02d)\n", arr[i], tarr[i]);
    // fflush(stdout);

    bool match = true; // Assume que já estão ordenados e percorre procurando algum valor que não seja igual
    for (int i = 0; i < N && match; i++) {
        if (arr[i] != tarr[i]) {
            match = false;
        }
    }

    // Desaloca os arranjos auxiliares
    free(tarr);
    free(arr);

    return match;
}

/**
 * @brief Imprime uma representação textual da estrutura geometrica completa de uma subárvore BST deitada no terminal.
 * 
 * @details Utiliza um percurso invertido (Direita -> Raiz -> Esquerda) e uma identação progressiva
 *          para desenhar a árvore "deitada". A raiz aparece na margem esquerda do terminal,
 *          a subárvore direita cresce para a parte de cima e a esquerda para a parte de baixo.
 * 
 * @param[in] root Ponteiro para o nó raiz da subárvore a ser impressa.
 * @param[in] space Quantidade atual de espaços (identação) para o nível de profundidade atual.
 */
void bst_printTreeRec(Node *root, int space) {
    if (root) {
        space += TAB_SIZE;

        bst_printTreeRec(root->right, space);

        printf("\n");
        for (int i = TAB_SIZE; i < space; i++) {
            printf(" ");
        }
        printf("%02d\n", root->key);

        bst_printTreeRec(root->left, space);
    }
}

/**
 * @brief Imprime uma representação textual da estrutura geometrica completa de uma árvore BST deitada no terminal.
 * 
 * @param[in] T Ponteiro para a árvore BST a ser representada.
 */
void bst_printTree(BST *T) {
    bst_printTreeRec(T->root, 0);
}


/*------------------------------------------------------------------------------
 * Permutação de um Arranjo
 *
 * Implementan o algoritmo iterativo de Narayana Pandita para a permutação de
 * um arranjo em ordem lexicográfica.
 */

/**
 * @brief Troca o valor de duas variáveis inteiras.
 * 
 * @param[in] a Ponteiro para a primeira variável inteira a ser permutada.
 * @param[in] b Ponteiro para a segunda variável inteira a ser permutada.
 */
void swap(int *a, int *b) {
    const int temp = *a;
    *a = *b;
    *b = temp;
}

/**
 * @brief Inverte a ordem dos elementos de um sub-arranjo (uma parte) específico dentro de um vetor.
 * 
 * @details Utiliza dois índices que convergem para o centro do sub-arranjo. A cada iteração,
 *          os elementos das extremidades opostas são trocados utilizando a função `swap()`.
 * 
 * @param[in,out] arr Ponteiro para o vetor contendo o sub-arranjo a ser invertido
 *                    (a alteração é feita no próprio vetor original).
 * @param[in] inicio Índice inicial (inclusivo) do sub-arranjo onde a inversão começará.
 * @param[in] fim Índice final (inclusivo) do sub-arranjo onde a inversão terminará.
 */
void perm_invert(int *arr, int inicio, int fim) {
    while (inicio < fim) {
        swap(&arr[inicio], &arr[fim]);
        inicio++;
        fim--;
    }
}

/**
 * @brief Gera a próxima permutação lexicográfica de um arranjo.
 * 
 * @details Implementa o algoritmo de Narayana Pandita. O algoritmo 
 *          se inicia encontrando o maior índice `i` tal que `arr[i] < arr[i+1]`; 
 *          Depois, encontra o maior índice `j` > `i` onde `arr[j] > arr[i]`;
 *          Posteriormente, troca os elementos de `i` e `j` usando `swap()`;
 *          Por fim, inverte a ordem do sufixo a partir de `i+1` usando `perm_invert()`.
 *          Se o arranjo estiver em ordem decrescente (a última permutação
 *          possível), a função retorna false.
 * 
 * @param[in,out] arr Ponteiro para o arranjo de inteiros que será permutado.
 * @param[in] tamanho Quantidade de elementos contidos no arranjo.
 * 
 * @return Status da geração da nova permutação.
 * 
 * @retval - `true`: se uma nova permutação foi gerada com sucesso;
 * @retval - `false`: se não houver mais permutações possíveis (o arranjo 
 *         chegou à sua ordem reversa máxima).
 */
bool perm_next(int *arr, int tamanho) {
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

/**
 * @brief Imprime uma representação textual de um arranjo de inteiros ("data").
 * 
 * @param[in] data Ponteiro para o arranjo a ser impresso.
 * @param[in] N Quantidade de elementos contidos no arranjo `data`.
 */
void data_print(const int *const data, const int N) {
    printf("data: [ ");
    for (int i = 0; i < N; i++) {
        printf("%02d ", data[i]);
    }
    printf("]\n");
}

/**
 * @brief Remove todas as ocorrências de um valor específico em um arranjo.
 * 
 * @details A remoção é feita sobrescrevendo o elemento alvo e deslocando todos os 
 *          elementos subsequentes uma posição para a esquerda. O índice do loop é 
 *          decrementado (i--) após uma remoção para que valores adjacentes idênticos 
 *          não sejam ignorados após um deslocamento.
 * 
 * @param[in,out] arr Ponteiro para o arranjo onde a remoção ocorrerá.
 * @param[in] N Tamanho atual do arranjo.
 * @param[in] value O valor numérico que será procurado e removido do arranjo.
 * 
 * @return O novo tamanho do arranjo (`N` - a quantidade de elementos removidos).
 */
int arr_remove(int *arr, int N, int value) {
    for (int i = 0; i < N; i++) {
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
    int DATA_BASE[] = {1, 2, 3, 4, 5};
    const int N = sizeof(DATA_BASE) / sizeof(int); // Tamanho do arranjo `DATA_BASE`

    qsort(DATA_BASE, N, sizeof(int), int_comp); // Ordena o arranjo `DATA_BASE`

    int *data_insert, *data_remove;

    data_insert = (int *) malloc(sizeof(int) * N);
    data_remove = (int *) malloc(sizeof(int) * N);
    int *arr = (int *) malloc(sizeof(int) * N);

    memcpy(data_insert, DATA_BASE, sizeof(int) * N);

    unsigned long long permutacoesTestadas = 0ULL;
    
    /**
     * Desativamos as impressões para deixar o assert fazer a verificação automaticamente
     */

    do { // Loop de Inserção
        memcpy(data_remove, DATA_BASE, sizeof(int) * N);

        do { // Loop de Remoção
            BST *T = bst_alloc();

            // printf("--------------------------------------------\n");
            // printf("Dados para Insercao:\n\t");
            // data_print(data_insert, N);
            
            for (int i = 0; i < N; i++) {
                // printf("Inserindo: %02d\n", data_insert[i]);
                Node *nd = node_alloc(data_insert[i]);
                avl_insert(T, nd);
                // bst_print(T);
                assert(bst_check(T->root, data_insert, i + 1));
            }
            
            // printf("Arvore apos todas as INSERCOES:\n");
            // bst_printTree(T);

            // printf("Dados para Remocao:\n\t");
            // data_print(data_remove, N);
            int asize = N;
            memcpy(arr, data_insert, sizeof(int) * asize);
            
            for (int i = 0; i < N; i++) {
                // printf("Removendo: %02d\n", data_remove[i]);
                avl_delete(T, bst_search(T->root, data_remove[i]));
                // bst_print(T);
                asize = arr_remove(arr, asize, data_remove[i]);
                assert(bst_check(T->root, arr, asize));
            }
            
            // printf("Arvore apos todas as REMOCOES:\n");
            // bst_printTree(T);

            bst_free(T);

            permutacoesTestadas++;
        } while (perm_next(data_remove, N));
    } while (perm_next(data_insert, N));
    
    free(data_remove);
    free(data_insert);
    free(arr);

    printf("\n%llu permutações distintas entre inserções / remoções foram validadas com sucesso!\n", permutacoesTestadas);

    return EXIT_SUCCESS;
}
