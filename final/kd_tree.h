#ifndef KD_TREE_H
#define KD_TREE_H

typedef struct _no {
    double ponto[2], ibge, capital, siafi_id, ddd;
    char nome[80], fuso[80];
    double codigo_uf;
    struct _no* esq;
    struct _no* dir;
} tno;


#endif
