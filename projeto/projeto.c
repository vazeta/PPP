#include "operacoes.h"
#include "ficheiros.h"
//  usar make
//  ou compilar com gcc -Wall -Wextra -o projeto ficheiros_valid.c operacoes.c projeto.c
//  executar com  ./projeto

//Função responsável por apresentar um menu e chamar as funções necessárias
void menu(Doente_header *lista_doentes){
    int opcao;
    printf("\n-------------------BEM VINDO AO DOC_HELPER_PPP - UMA APLICAÇÃO QUE O AJUDA A GERIR OS SEU DOENTES -----------\n");
    do {
        printf("------------------------------------------------------------------------------------------------------------\n");
        printf("---------------------------------------------------------Menu----------------------------------------------\n");
        printf("1. Introduzir um novo doente\n");
        printf("2. Eliminar um doente existente\n");
        printf("3. Listar todos os doentes por ordem alfabética\n");
        printf("4. Listar os doentes com tensões máximas acima de um determinado valor\n");
        printf("5. Apresentar toda a informação de um determinado doente\n");
        printf("6. Registar as tensões, o peso e a altura de um determinado doente\n");
        printf("7. Sair da aplicação\n");
        printf("\nEscolha uma das operações possiveis: ");
        scanf("%d", &opcao);
        getchar();

        if (opcao < 1 || opcao > 7) {
            printf("Opcao invalida. Tente novamente.\n");
            continue;
        }
        switch (opcao) {
            case 1:
                introduzir_doente(lista_doentes);
                break;
            case 2:
                eliminar_doente(lista_doentes);
                break;
            case 3:
                listar_doentes_alf(lista_doentes);
                break;
            case 4:
                listar_doentes_tensao_sup(lista_doentes);
                break;
            case 5:
                info_doente(lista_doentes);
                break;
            case 6:
                registar_dados(lista_doentes);
                break;
            case 7:
                printf("A sair....\n");
                free(lista_doentes);
                break;
            default:
                printf("Opcao invalida. Tente novamente.\n");
                break;
        }
    } while (opcao != 7);
}


//main(). Inicializa a lista. Lê os ficheiros e chama a função menu
int main(){
    Doente_header *lista_doentes = (Doente_header *)malloc(sizeof(Doente_header));
    if(lista_doentes==NULL){
        printf("Erro ao alocar memória\n");
        return 1;
    }
    inicializar_lista(lista_doentes);
    ler_ficheiro_doentes(lista_doentes);
    ler_ficheiro_registos(lista_doentes);
    menu(lista_doentes);
    return 0;
}