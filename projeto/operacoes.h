#ifndef OPERACOES_H
#define OPERACOES_H
#include "structs.h"

int ver_ultimo_id(Doente_header *lista_doentes);
void inicializar_lista(Doente_header *lista_doentes);
int verifica_existencia(Doente_header *lista_doentes, int id);
void introduzir_doente(Doente_header *lista_doentes);
void eliminar_doente(Doente_header *lista_doentes);
void listar_doentes_alf(Doente_header *lista_doentes);
void inserir_doente_na_lista_aux(aux_header *lista_aux, Doente *novo_doente);
void listar_doentes_tensao_sup(Doente_header *lista_doentes);
void info_doente(Doente_header *lista_doentes);
void registar_dados(Doente_header *lista_doentes);

#endif