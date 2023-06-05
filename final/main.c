#include <stdio.h>
#include <stdlib.h>
#include "kd_tree.c"

tno* csv(const char* arq) {
    FILE* arquivo = fopen(arq, "r"); // abre arquivo para leitura
    if (arquivo == NULL) {
        printf("erro\n");
        return NULL;
    }

    char linha[100];
    int primeira_linha = 1;  // auxiliar
    tno* raiz = NULL;

    while (fgets(linha, sizeof(linha), arquivo) != NULL) {  // percorre o arquivo
        if (primeira_linha) {
            primeira_linha = 0;  // pular a primeira linha
            continue;
        }

        tno* novo_no = (tno*)malloc(sizeof(tno));  // cria um novo nó
        //memset(novo_no, 0, sizeof(tno));


        // atribuições
        char* token = strtok(linha, ",");
        if (token != NULL) {
            novo_no->ibge = atof(token);
        }

        token = strtok(NULL, ",");
        if (token != NULL) {
            strcpy(novo_no->nome, token);
        }

        token = strtok(NULL, ",");
        if (token != NULL) {
            novo_no->ponto[0] = atof(token);
        }

        token = strtok(NULL, ",");
        if (token != NULL) {
            novo_no->ponto[1] = atof(token);
        }

        token = strtok(NULL, ",");
        if (token != NULL) {
            novo_no->capital = atof(token);
        }

        token = strtok(NULL, ",");
        if (token != NULL) {
        novo_no->codigo_uf = atof(token);
        }

        token = strtok(NULL, ",");
        if (token != NULL) {
            novo_no->siafi_id = atof(token);
        }

        token = strtok(NULL, ",");
        if (token != NULL) {
            novo_no->ddd = atof(token);
        }

        token = strtok(NULL, ",");
        if (token != NULL) {
            strncpy(novo_no->fuso, token, sizeof(novo_no->fuso) - 1);
        }

        raiz = inserir_no(raiz, novo_no, 0);
    }

    fclose(arquivo);
    return raiz;
}


int main() {
    const char* arq = "municipios.csv";
    tno* raiz = csv(arq);

    double xt = -20;
    double yt = -5;

    tno* pontot = encontrar_vizinho(raiz, xt, yt);

    if (pontot != NULL) {
        printf("A cidade mais próxima é:\n");
        printf("IBGE: %0.0f\n", pontot->ibge);
        printf("Nome do município: %s\n", pontot->nome);
        printf("Código UF: %0.0f\n", pontot->codigo_uf);
        printf("Capital: %0.0f\n", pontot->capital);
        printf("Latitude: %0.6f, Longitude: %.6f\n", pontot->ponto[0], pontot->ponto[1]);
        printf("Código SIAFI: %0.0f\n", pontot->capital);
        printf("DDD: %0.0f\n", pontot->ddd);
        printf("Fuso: %s", pontot->fuso);
    } else {
        printf(":(\n");
    }

    return 0;
}

