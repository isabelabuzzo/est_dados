#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include "kd_tree.h"

double distancia(double x1, double y1, double x2, double y2) { // função que calcula distância entre nós
    double dx = x1 - x2;                                        // distância em x
    double dy = y1 - y2;                                        // distância em y
    double d = sqrt(dx * dx + dy * dy);                         // √(x^2 + y^2)
    return d;
}

tno* criar_no(double x, double y) {                 // função para criar nova árvore
    tno* novo_no = (tno*)malloc(sizeof(tno));       // aloca espaço para novo nó
    novo_no->ponto[0] = x;
    novo_no->ponto[1] = y;
    novo_no->esq = NULL;                            // atribuindo parâmetros
    novo_no->dir = NULL;
    return novo_no;
}

tno* inserir_no(tno* arv, tno* no_calc,int eixo) {              // função para inserir um nó
    if (arv == NULL) {                                          // se a arv não existe, é criada
        return criar_no(no_calc->ponto[0], no_calc->ponto[1]);
    }

    int eixo_atual = eixo % 2;                                  // calcula qual o eixo atual

    if (eixo_atual == 0) {                          // se o eixo é x
        if (no_calc->ponto[0] < arv->ponto[0]) {        
            arv->esq = inserir_no(arv->esq, no_calc, eixo + 1); // se x é menor, vai para esquerda
        } else {
            arv->dir = inserir_no(arv->dir, no_calc, eixo + 1); // se x é maior, vai para direita
        }
    } else {
        if (no_calc->ponto[1] < arv->ponto[1]) {
            arv->esq = inserir_no(arv->esq, no_calc, eixo + 1);
        } else {
            arv->dir = inserir_no(arv->dir, no_calc, eixo + 1);
        }
    }

    strncpy(arv->nome, no_calc->nome, sizeof(arv->nome) - 1);
    strncpy(arv->fuso, no_calc->fuso, sizeof(arv->fuso) - 1);
    arv->capital = no_calc->capital;
    arv->ddd = no_calc->ddd;
    arv->ibge = no_calc->ibge;
    arv->siafi_id = no_calc->siafi_id;
    arv->codigo_uf = no_calc->codigo_uf;

    return arv;
}


tno* encontrar_vizinho(tno* arv, double x, double y) {   // encontrar vizinho mais próximo
    tno* mais_proximo = NULL;
    double menor_distancia = INFINITY;
    tno* no_atual = arv;

    while (no_atual != NULL) {
        double dist = distancia(x, y, no_atual->ponto[0], no_atual->ponto[1]);  // calcula distancia da raiz até o ponto

        if (dist < menor_distancia) {  // se a distancia é menor, atualiza
            menor_distancia = dist;
            mais_proximo = no_atual;
        }

        if (x < no_atual->ponto[0] || (x == no_atual->ponto[0] && y < no_atual->ponto[1])) {
            no_atual = no_atual->esq;
        } else {   // se x é maior
            no_atual = no_atual->dir;
        }
    }

    return mais_proximo;
}
