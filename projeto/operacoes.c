#include "includes.h"
#include "ficheiros.h"
#include "operacoes.h"

//Função responsável por inicalizar a lista de doentes 
void inicializar_lista(Doente_header *lista_doentes){
    lista_doentes->start = NULL;
}

//Função responsável por returnal o proximo id disponivel
int ver_ultimo_id(Doente_header *lista_doentes){
    int id_a_usar=0;
    Doente *atual=lista_doentes->start;
    while(atual!=NULL){ 
        if(atual->id_doente > id_a_usar){
            id_a_usar=atual->id_doente;
        }
        atual=atual->prox;
    }
    return id_a_usar+1;
}

//Função responsável por verificar a existencia de um doente através do id
int verifica_existencia(Doente_header *lista_doentes, int id){
    Doente *atual=lista_doentes->start;
    while(atual!=NULL){
        if(atual->id_doente==id){
            return 1;
        }
        atual=atual->prox;
    }
    return 0;
}

//Função responsável por introduzir um novo doente
void introduzir_doente(Doente_header *lista_doentes) {
    Doente *novo = (Doente *)malloc(sizeof(Doente));
    Doente *atual = lista_doentes->start;
    Doente *anterior = NULL;

    novo->id_doente = ver_ultimo_id(lista_doentes);
    printf("Introduza os dados do novo doente\n");

    if (!verifica_existencia(lista_doentes, novo->id_doente)) {
        do {
            printf("Nome completo: ");
            fgets(novo->nome, MAX_TAM, stdin);
            novo->nome[strcspn(novo->nome, "\n")] = '\0';
            if (!validar_nome(novo->nome)) {
                printf("Nome inválido. Por favor, introduza um nome completo.\n");
            }
        } while (!validar_nome(novo->nome));
        int data_valida = 0;
        int res = 0;
        while (!data_valida || res != 3) {
            res = 0;
            printf("Data de nascimento (dia/mes/ano): ");
            res = scanf("%d/%d/%d", &novo->data_nasc.dia, &novo->data_nasc.mes, &novo->data_nasc.ano);
            getchar();
            data_valida = validar_data(novo->data_nasc.dia, novo->data_nasc.mes, novo->data_nasc.ano);
            if (!data_valida || res != 3) {
                printf("Data inválida\n");
            }
        }
        do {
            printf("Cartão de cidadão: ");
            fgets(novo->CC, MAX_TAM, stdin);
            novo->CC[strcspn(novo->CC, "\n")] = '\0';
            if (!validar_cc(novo->CC)) {
                printf("Cartão de Cidadão inválido. Por favor, introduza um número de CC válido.\n");
            }
        } while (!validar_cc(novo->CC));


        do {
            printf("Telemóvel: ");
            scanf("%d", &novo->telemovel);
            getchar();
            if (!validar_numero(novo->telemovel)) {
                printf("Número de telemóvel inválido. Por favor, introduza um número começando por 9 e com 9 dígitos.\n");
            }
        } while (!validar_numero(novo->telemovel));

        do {
            printf("Email: ");
            fgets(novo->email, MAX_TAM, stdin);
            novo->email[strcspn(novo->email, "\n")] = '\0';
            if (!validar_email(novo->email)) {
                printf("Email inválido. Por favor, introduza um email válido.\n");
            }
        } while (!validar_email(novo->email));

       
        novo->prox = NULL;
        novo->registos = (Registo_header *)malloc(sizeof(Registo_header));
        novo->registos->start = NULL;

        
        while (atual != NULL && strcmp(atual->nome, novo->nome) < 0) {
            anterior = atual;
            atual = atual->prox;
        }

        if (anterior == NULL) {
            novo->prox = lista_doentes->start;
            lista_doentes->start = novo;
        } else {
            anterior->prox = novo;
            novo->prox = atual;
        }

        escrever_ficheiro_doentes(novo);
    } else {
        printf("O paciente que pretende introduzir já existe\n");
        free(novo);
    }
}

//Função responsável por eliminar um doente
void eliminar_doente(Doente_header *lista_doentes){
    int id_a_eliminar;
    listar_doentes_alf(lista_doentes);
    printf("Introduza o ID do cliente a eliminar: ");
    scanf("%d", &id_a_eliminar);
    if(verifica_existencia(lista_doentes,id_a_eliminar)){
        Doente *atual = lista_doentes->start;
        Doente *prev = NULL;
        while (atual != NULL){
            if (atual->id_doente == id_a_eliminar){
                if (prev != NULL) { 
                    prev->prox = atual->prox;
                } else {
                    lista_doentes->start = atual->prox; 
                }
                free(atual);
                apagar_ficheiro_doentes(id_a_eliminar);
                apagar_ficheiro_registos(id_a_eliminar);
                break;
            }
            prev = atual;
            atual = atual->prox;
        }
    }else{
        printf("O cliente que pretende eliminar não existe\n");
    }
}

//Função responsável por listar os doentes por ordem alfabética (imprime a lista)
void listar_doentes_alf(Doente_header *lista_doentes){
    Doente *atual=lista_doentes->start;
    while(atual!=NULL){
        printf("%d - %s\n", atual->id_doente, atual->nome);
        atual=atual->prox;
    }
}

//Função responsável por inserir os doentes na lista auxiliar utilizada para listar os doentes com uma certa tensao maxima superior
void inserir_doente_na_lista_aux(aux_header *lista_aux, Doente *novo) {
    aux *atual = lista_aux->start;
    aux *novo_node = (aux *)malloc(sizeof(aux));
    if (novo_node == NULL) {
        printf("Erro ao alocar memória.\n");
        exit(1);
    }
    novo_node->doente = novo;
    novo_node->prox = NULL;
    if (atual == NULL || (atual!=NULL && novo->registos->start->tensao_max > atual->doente->registos->start->tensao_max)) {
        novo_node->prox = lista_aux->start;
        lista_aux->start = novo_node;
        return;
    }

    aux *anterior = NULL;
    while (atual != NULL && atual->doente->registos->start->tensao_max > novo->registos->start->tensao_max) {
        anterior = atual;
        atual = atual->prox;
    }
    if (atual != NULL) {
        novo_node->prox = atual;
        anterior->prox = novo_node;
    } else {
        anterior->prox = novo_node;
    }
}

//Função responsável por listar os doentes com uma certa tensao maxima superior
void listar_doentes_tensao_sup(Doente_header *lista_doentes) {
    aux_header *lista_aux = (aux_header *)malloc(sizeof(aux_header));
    lista_aux->start=NULL;
    int tensao_a_analisar;
    printf("Pretende listar os doentes com tensões máximas superiores a: ");
    scanf("%d", &tensao_a_analisar);
    Doente *doente_atual = lista_doentes->start;
    while (doente_atual != NULL) {
        if (doente_atual->registos->start != NULL && doente_atual->registos->start->tensao_max > tensao_a_analisar) {
            inserir_doente_na_lista_aux(lista_aux, doente_atual);
        }
        doente_atual = doente_atual->prox;
    }
    aux *aux_atual = lista_aux->start;
    while (aux_atual != NULL) {
        printf("O doente %s apresentou as seguintes tensões maximas maiores que %d:\n", aux_atual->doente->nome, tensao_a_analisar);
        Registo *registo_atual = aux_atual->doente->registos->start;
        while (registo_atual != NULL && registo_atual->tensao_max > tensao_a_analisar) {
            printf("- Tensão máxima: %d no dia %d/%d/%d\n", registo_atual->tensao_max, registo_atual->data_res.dia, registo_atual->data_res.mes, registo_atual->data_res.ano);
            registo_atual = registo_atual->prox;
        }
        aux_atual = aux_atual->prox;
    }
    free(lista_aux);
}

//Função responsável por apresentar todas as informações de um doente
void info_doente(Doente_header *lista_doentes) {
    int id;
    listar_doentes_alf(lista_doentes);
    printf("Indroduza o id do paciente:");
    scanf("%d", &id);
    if (verifica_existencia(lista_doentes, id)) {
        Doente *atual = lista_doentes->start;

        while (atual != NULL) {
            if (atual->id_doente == id) {
                printf("Doente %d\n", atual->id_doente);
                printf("Nome:%s\n", atual->nome);
                printf("Nasceu a: %d/%d/%d\n", atual->data_nasc.dia, atual->data_nasc.mes, atual->data_nasc.ano);
                printf("Cartão de cidadão:%s\n", atual->CC);
                printf("Numero de telemovel %d\n", atual->telemovel);
                printf("Email:%s\n", atual->email);
                Registo *registro_atual = atual->registos->start;
                while (registro_atual != NULL) {
                    printf("No dia %d/%d/%d ", registro_atual->data_res.dia, registro_atual->data_res.mes, registro_atual->data_res.ano);
                    printf("registou uma tensao maxima de %d e minima de %d, ", registro_atual->tensao_max, registro_atual->tensao_min);
                    printf("registou um peso de %d kg e uma altura de %d cm\n", registro_atual->peso, registro_atual->altura);
                    registro_atual = registro_atual->prox;
                }
                break;
            }
            atual = atual->prox;
        }
    } else {
        printf("O Doente introduzido não existe\n");
    }
}


// Função para registrar dados de um doente
void registar_dados(Doente_header *lista_doentes) {
    Registo *novo = (Registo *)malloc(sizeof(Registo));
    Doente *atual_doente = lista_doentes->start;
    int id_novo;

    printf("Introduza o novo registo do doente\n");
    printf("ID: ");
    scanf("%d", &id_novo);
    getchar();

    if (verifica_existencia(lista_doentes, id_novo)) {
        int data_valida = 0;
        int res = 0;

        while (!data_valida || res != 3) {
            res = 0;
            printf("Data de registo (dia/mes/ano): ");
            res = scanf("%d/%d/%d", &novo->data_res.dia, &novo->data_res.mes, &novo->data_res.ano);
            getchar();
            data_valida = validar_data(novo->data_res.dia, novo->data_res.mes, novo->data_res.ano);
            if (!data_valida || res != 3) {
                printf("Data inválida\n");
            }
        }

        printf("Tensão máxima: ");
        scanf("%d", &novo->tensao_max);
        getchar();

        do {
            printf("Tensão mínima: ");
            scanf("%d", &novo->tensao_min);
            getchar();
            if (!validar_tensao(novo->tensao_max, novo->tensao_min)) {
                printf("Tensão mínima acima da tensão máxima. Por favor, introduza novamente.\n");
            }
        } while (!validar_tensao(novo->tensao_max, novo->tensao_min));

        printf("Peso: ");
        scanf("%d", &novo->peso);
        getchar();

        printf("Altura: ");
        scanf("%d", &novo->altura);
        getchar();

        novo->prox = NULL;

        while (atual_doente != NULL && atual_doente->id_doente != id_novo) {
            atual_doente = atual_doente->prox;
        }

        if (atual_doente != NULL) {
            Registo *anterior = NULL;
            Registo *atual_registo = atual_doente->registos->start;

            while (atual_registo != NULL && novo->tensao_max < atual_registo->tensao_max) {
                anterior = atual_registo;
                atual_registo = atual_registo->prox;
            }

            if (anterior == NULL) {
                novo->prox = atual_doente->registos->start;
                atual_doente->registos->start = novo;
            } else {
                anterior->prox = novo;
                novo->prox = atual_registo;
            }

            escrever_ficheiro_registos(novo, id_novo);
        } else {
            printf("Erro: ID de paciente não encontrado na lista de doentes.\n");
            free(novo);
        }
    } else {
        printf("Erro: ID de paciente não encontrado na lista de doentes.\n");
        free(novo);
    }
}