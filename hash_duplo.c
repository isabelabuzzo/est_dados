#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#define SEED    0x12345678


// adicionados campos do municipio (==)
typedef struct {
    char ibge[8];
    char nome[100];
    char latitude[15];
    char longitude[15];
    char capital[2];
    char codigoUF[3];
    char siafi[7];
    char ddd[2];
    char fuso[50];
} municipio;

// obter chave do município  (==)
char *get_key_municipio(void *reg) {
    return ((municipio *)reg)->ibge;
}

// aloca memoria (==)
municipio *aloca_municipio(char *ibge, char *nome, char *latitude, char *longitude, char *capital, char *codigoUF, char *siafi, char *ddd, char *fuso) {
    municipio *m = malloc(sizeof(municipio));
    strcpy(m->ibge, ibge);
    strcpy(m->nome, nome);
    strcpy(m->latitude, latitude);
    strcpy(m->longitude, longitude);
    strcpy(m->capital, capital);
    strcpy(m->codigoUF, codigoUF);
    strcpy(m->siafi, siafi);
    strcpy(m->ddd, ddd);
    strcpy(m->fuso, fuso);
    return m;
}

// adicionar table2 para hash duplo (==)
typedef struct {
    uintptr_t *table;
    uintptr_t *table2;  // resolução de colisões
    int size;           
    int max;
    uintptr_t deleted;
    char *(*get_key)(void *);
} thash;


// produzir valor de hash
uint32_t hashf(const char *str, uint32_t h, uint32_t h2) {
    /* One-byte-at-a-time Murmur hash */ // algoritmo de hash
    while (*str) {
        h ^= *str++;
        h *= 0x5bd1e995;
        h ^= h >> 15; 
    }
    return h + (h2 % 2 + 1); // posicao final do hash
}

int hash_insere(thash *h, void *bucket) {
    uint32_t hash = hashf(h->get_key(bucket), SEED, SEED);
    int pos = hash % h->max;
    int pos2 = h->max - 2 - (hash % (h->max - 2));  // pos2

    if (h->max == (h->size + 1)) {  // se a tabela esta cheia
        free(bucket);               // libera a memória
        return EXIT_FAILURE;
    } else {
        while (h->table[pos] != 0) {        // enquanto posição n for vazia
            if (h->table[pos] == h->deleted)    // posição deletada
                break;
            pos = (pos + 1) % h->max;               // proxima posição
            pos2 = (pos2 + 1) % h->max;             // pos2
        }
        h->table[pos] = (uintptr_t)bucket;          // insere elemento
        h->table2[pos2] = (uintptr_t)bucket;        // pos2 
        h->size += 1;
    }
    return EXIT_SUCCESS;
}

// construir tabela hash
int hash_constroi(thash *h, int nbuckets, char *(*get_key)(void *)) {
    h->table = calloc(sizeof(uintptr_t), nbuckets + 1);     // alocar memória
    h->table2 = calloc(sizeof(uintptr_t), nbuckets + 1);    // alocar table2
    if (h->table == NULL || h->table2 == NULL) {            // add table2
        return EXIT_FAILURE;
    }
    h->max = nbuckets + 1;          // tamanho max tabela
    h->size = 0;                    // contador de elemento
    h->deleted = (uintptr_t)&(h->size);         // marcador de elementos
    h->get_key = get_key;                          // obter chave
    return EXIT_SUCCESS;
}

void *hash_busca(thash h, const char *key) {
    uint32_t hash = hashf(key, SEED, SEED);
    int pos = hash % h.max;
    int pos2 = h.max - 2 - (hash % (h.max - 2));     // pos2

    while (h.table[pos] != 0) {     // enquanto posição n for vazia
        if (strcmp(h.get_key((void *)h.table[pos]), key) == 0) {        // comparar chaves
            return (void *)h.table[pos];          // retorna elemento
        } else {
            pos = (pos + 1) % h.max;                // proxima posição da tabela
            pos2 = (pos2 + 1) % h.max;                                  // pos2
        }
    }
    return NULL;
}

// remover elemento
int hash_remove(thash *h, const char *key) {
    int pos = hashf(key, SEED, SEED) % (h->max);
    int pos2 = h->max - 2 - (hashf(key, SEED, SEED) % (h->max - 2));   // pos2

    while (h->table[pos] != 0) {        // enquanto posição n for vazia
        if (strcmp(h->get_key((void *)h->table[pos]), key) == 0) {      // comparar chaves
            free((void *)h->table[pos]);        // libera a memória do elemento encontrado
            h->table[pos] = h->deleted;         // posição excluida
            h->table2[pos2] = h->deleted;       // pos2
            h->size -= 1;                       // num de elementos
            return EXIT_SUCCESS;
        } else {
            pos = (pos + 1) % h->max;           // proxima posicao
            pos2 = (pos2 + 1) % h->max;         // pos2
        }
    }
    return EXIT_FAILURE;
}

// desalocar tabela
void hash_apaga(thash *h) {
    int pos;
    for (pos = 0; pos < h->max; pos++) {            // percorre todas as posições da tabela
        if (h->table[pos] != 0) {                   // se posicao n é vazia
            if (h->table[pos] != h->deleted) {      // se posição n foi excluida
                free((void *)h->table[pos]);        // libera memoria
            }
        }
    }
    free(h->table);     // libera memoria

}

void carrega_municipios(thash *h, const char *nome_arquivo) {
    FILE *arquivo = fopen(nome_arquivo, "r");
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo.\n");
        return;
    }

    char linha[5580];
    while (fgets(linha, sizeof(linha), arquivo)) {
        char *token = strtok(linha, ",");
        char ibge[8];
        char nome[100];
        char latitude[15];
        char longitude[15];
        char capital[2];
        char codigoUF[3];
        char siafi[7];
        char ddd[2];
        char fuso[50];

        strncpy(ibge, token, sizeof(ibge));
        token = strtok(NULL, ",");

        strncpy(nome, token, sizeof(nome));
        token = strtok(NULL, ",");

        strncpy(latitude, token, sizeof(latitude));
        token = strtok(NULL, ",");

        strncpy(longitude, token, sizeof(longitude));
        token = strtok(NULL, ",");

        strncpy(capital, token, sizeof(capital));
        token = strtok(NULL, ",");

        strncpy(codigoUF, token, sizeof(codigoUF));
        token = strtok(NULL, ",");

        strncpy(siafi, token, sizeof(siafi));
        token = strtok(NULL, ",");

        strncpy(ddd, token, sizeof(ddd));
        token = strtok(NULL, ",");

        strncpy(fuso, token, sizeof(fuso));
        token = strtok(NULL, ",");

        municipio *m = aloca_municipio(ibge, nome, latitude, longitude, capital, codigoUF, siafi, ddd, fuso);
        hash_insere(h, m);
    }

    fclose(arquivo);
}

void buscarMunicipio(thash h, const char *ibge_busca) {
    municipio *municipio_busca = hash_busca(h, ibge_busca);
    if (municipio_busca != NULL) {
        printf("Município encontrado!\n");
        printf("IBGE: %s\n", municipio_busca->ibge);
        printf("Nome: %s\n", municipio_busca->nome);
        printf("Latitude: %s\n", municipio_busca->latitude);
        printf("Longitude: %s\n", municipio_busca->longitude);
        printf("Capital: %s\n", municipio_busca->capital);
        printf("Código UF: %s\n", municipio_busca->codigoUF);
        printf("Siafi: %s\n", municipio_busca->siafi);
        printf("Fuso: %s\n", municipio_busca->fuso);
    } else {
        printf("Município não encontrado.\n");
    }
}



int main() {
    thash municipios;

    hash_constroi(&municipios, 5570, get_key_municipio);

    carrega_municipios(&municipios, "municipios.csv");


    // busca por um município
    buscarMunicipio(municipios, "2400505");
    buscarMunicipio(municipios, "5106265");
    buscarMunicipio(municipios, "3303807");
    buscarMunicipio(municipios, "2211407");
    buscarMunicipio(municipios, "3557154");


    // remover município
    char ibge_remove[] = "3557154";
    int removido = hash_remove(&municipios, ibge_remove);
    if (removido == EXIT_SUCCESS) {
        printf("Município removido com sucesso.\n");
    } else {
        printf("Falha ao remover o município.\n");
    }

    hash_apaga(&municipios);

    return 0;
}
