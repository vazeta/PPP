#ifndef FICHEIROS_H
#define FICHEIROS_H
#include "structs.h"

int validar_data(int dia, int mes,int ano);
int validar_nome(char *nome);
int validar_email(char *email);
int validar_cc(char *cc);
int validar_numero(int num);
int validar_tensao(int tensao_max, int tensao_min);
void ler_ficheiro_doentes(Doente_header *lista_doentes);
void ler_ficheiro_registos(Doente_header *lista_doentes);
void escrever_ficheiro_doentes(Doente *novo);
void escrever_ficheiro_registos(Registo *novo, int id_novo_registo);
void apagar_ficheiro_doentes(int id);
void apagar_ficheiro_registos(int id);

#endif