#ifndef STRUCTS_H
#define STRUCTS_H
#include "includes.h"
#define MAX_TAM 30

typedef struct data{
    int dia;
    int mes;
    int ano;
}Data;

typedef struct registo{
    Data data_res;
    int tensao_max;
    int tensao_min;
    int peso;
    int altura;
    struct registo *prox;
}Registo;

typedef struct registo_header{
    Registo *start;
}Registo_header;

typedef struct doente{
    int id_doente;
    char nome [MAX_TAM];    
    Data data_nasc;
    char CC[MAX_TAM];
    int telemovel;
    char email[MAX_TAM];
    Registo_header *registos;
    struct doente *prox;
}Doente;

typedef struct doente_header{
    Doente *start;
}Doente_header;

typedef struct aux{
    Doente *doente;
    struct aux *prox;
}aux;

typedef struct aux_header{
    aux *start;
}aux_header;



#endif