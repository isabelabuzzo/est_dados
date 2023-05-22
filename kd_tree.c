#include <stdio.h>
#include <math.h>
#include <stdlib.h>

typedef struct _no {   // criação da estrutura do nó
    double ponto[2];   // coordenadas do ponto
    struct _no* dir;   // ponteiro para o nó esquerdo
    struct _no* esq;   // ponteiro para o nó da direita
    void* item;        // ponteiro para dados
} tno;

double distancia(double x1, double y1, double x2, double y2) {  // função que calcula distância entre nós
    double dx = x1 - x2;                  // distância em x
    double dy = y1 - y2;                  // distância em y
    double d = sqrt(dx * dx + dy * dy);         // √(x^2 + y^2)
    return d;
}


tno* criar_no(double x, double y) {             // função para criar novo nó
    tno* novoNo = (tno*)malloc(sizeof(tno));    // aloca espaço para novo nó
    novoNo->ponto[0] = x;       
    novoNo->ponto[1] = y;
    novoNo->dir = NULL;                         // atribuindo parâmetros
    novoNo->esq = NULL;
    return novoNo;

}

tno* inserirNo(tno* raiz, double x, double y, int eixo) {  // função para inserir um nó
    if (raiz == NULL) {               // se a raiz não existe, é criada
        return criar_no(x, y);
    }

    int eixo_atual = eixo % 2;        // calcula qual o eixo atual

    if (eixo_atual == 0){               // se o eixo é x
        if (x < raiz->ponto[0]){        // se x é menor, vai para esquerda
            raiz->esq = inserirNo(raiz->esq, x, y, eixo + 1);
        }                          
        else {                          // se x é maior, vai para direita
            raiz->dir = inserirNo(raiz->dir, x, y, eixo + 1);
        }
    }
    else {                              // se o eixo é y, o mesmo acontece
        if (y < raiz->ponto[1]){      
            raiz->esq = inserirNo(raiz->esq, x, y, eixo + 1);
        }                          
        else {                     
            raiz->dir = inserirNo(raiz->dir, x, y, eixo + 1);
        }
    }

    return raiz;

}

tno* encontrar_vizinho(tno* raiz, double x, double y) {
    tno* mais_proximo = NULL;
    double menor_distancia = INFINITY;
    tno* no_atual = raiz;

    while (no_atual != NULL) {
        double dist = distancia(x, y, no_atual->ponto[0], no_atual->ponto[1]);

        if (dist < menor_distancia){
            menor_distancia = dist;
            mais_proximo = no_atual;
        }

        if (x < no_atual->ponto[0] || (x == no_atual->ponto[0] && y < no_atual->ponto[1])){
            no_atual = no_atual->esq;
        }
        else {
            no_atual = no_atual->dir;
        }

    }

    return mais_proximo;
}