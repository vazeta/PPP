#include "includes.h"
#include "ficheiros.h"

//Função responsável por validar data
int validar_data(int dia, int mes, int ano) {
    if (ano < 0 || mes < 1 || mes > 12 || dia < 1) {
        return 0;
    }
    int dias_no_mes;
    switch(mes) {
        case 2: 
            if ((ano % 4 == 0 && ano % 100 != 0) || (ano % 400 == 0)) {
                dias_no_mes = 29;
            } else {
                dias_no_mes = 28;
            }
            break;
        case 4: case 6: case 9: case 11: 
            dias_no_mes = 30;
            break;
        default:
            dias_no_mes = 31;
            break;
    }
    return dia <= dias_no_mes;
}

// Função para validar o nome
int validar_nome(char *nome) {
    if (nome == NULL || strlen(nome) == 0) {
        return 0;
    }
    int count = 0;
    int length = (int) strlen(nome);
    for (int i = 0; i < length; i++) {
        if (isspace(nome[i])) {
            count++;
            if (i == 0 || i == length - 1 || isspace(nome[i + 1])) {
                return 0;
            }
            if (i + 1 < length && !isupper(nome[i + 1])) {
                return 0;
            }
        }
    }

    return count > 0 ? 1 : 0;
}

// Função para validar o email
int validar_email(char *email) {
    if (email == NULL || strlen(email) == 0) {
        return 0;
    }
    char *at_ptr = strstr(email, "@");
    if (at_ptr && strstr(at_ptr, ".")) {
        return 1;
    } else {
        return 0;
    }
}

// Função para validar o numero
int validar_numero(int num) {
    if (num >= 900000000 && num <= 999999999) {
        return 1;
    }
    return 0;
}

// Função para validar a tensão
int validar_tensao(int tensao_max, int tensao_min){
    if(tensao_max>tensao_min){
        return 1;
    }
    return 0;
}

// Função para validar o Cartão de Cidadão
int validar_cc(char *cc) {
    if (strlen(cc) != 14) {
        return 0;
    }
    for (int i = 0; i < 8; i++) {
        if (!isdigit(cc[i])) {
            return 0;
        }
    }
    if (cc[8] != '-') {
        return 0;
    }
    if (!isdigit(cc[9])) {
        return 0;
    }
    if (cc[10] != '-') {
        return 0;
    }

    if (!isalpha(cc[11]) || !isalpha(cc[12])) {
        return 0;
    }
    if (!isdigit(cc[13])) {
        return 0;
    }
    return 1;
}

//Funçao responsável por ler o ficheiro de doentes e carregar para a lista
void ler_ficheiro_doentes(Doente_header *lista_doentes) {
    FILE *file = fopen("doentes.txt", "r");
    char linha[MAX_TAM];
    int num_linhas = 0;
    if (file == NULL) {
        printf("Erro ao abrir o arquivo de doentes.\n");
        exit(-1);
    }

    while (fgets(linha, sizeof(linha), file) != NULL) {
        num_linhas++;
    }
    
    if (num_linhas % 6 != 0) {
        printf("Erro: O número de linhas no ficheiro de doentes não é valido.\n");
        fclose(file);
        exit(1);
    }
    rewind(file);
    Doente *novo = NULL;
    Doente *atual = lista_doentes->start;
    Doente *anterior = NULL;
    while (fgets(linha, sizeof(linha), file) != NULL) {
        if (strcmp(linha, "\n") == 0) {
            continue;
        }
        novo = (Doente *)malloc(sizeof(Doente));
        if (novo == NULL) {
            printf("Erro ao alocar memória para o novo doente.\n");
            fclose(file);
            return;
        }
        if (sscanf(linha, "%d", &novo->id_doente) == 1) {
            fgets(novo->nome, MAX_TAM, file);
            novo->nome[strcspn(novo->nome, "\n")] = '\0';
            if (!validar_nome(novo->nome)) {
                printf("Erro: Nome inválido no arquivo de texto.\n");
                fclose(file);
                free(novo);
                exit(1);
            }
            fgets(linha, sizeof(linha), file);
            sscanf(linha, "%d/%d/%d", &novo->data_nasc.dia, &novo->data_nasc.mes, &novo->data_nasc.ano);
            if (!validar_data(novo->data_nasc.dia, novo->data_nasc.mes, novo->data_nasc.ano)) {
                printf("Erro: Data de nascimento inválida no arquivo de texto.\n");
                fclose(file);
                free(novo);
                exit(1);
            }
            fgets(linha, sizeof(linha), file);
            sscanf(linha, "%s", novo->CC);
            if (!validar_cc(novo->CC)) {
                printf("Erro: Cartão de cidadao inválido no arquivo de texto.\n");
                fclose(file);
                free(novo);
                exit(1);
            }
            fgets(linha, sizeof(linha), file);
            sscanf(linha, "%d", &novo->telemovel);
            if (!validar_numero(novo->telemovel)) {
                printf("Erro: Número de telemóvel inválido no arquivo de texto.\n");
                fclose(file); 
                free(novo);
                exit(1);
            }
            fgets(linha, sizeof(linha), file);
            sscanf(linha, "%s", novo->email);
            if (!validar_email(novo->email)) {
                printf("Erro: Email inválido no arquivo de texto.\n");
                fclose(file);
                free(novo);
                exit(1);
            }
            novo->registos = (Registo_header *)malloc(sizeof(Registo_header));
            if (novo->registos == NULL) {
                printf("Erro ao alocar memória para os registos.\n");
                fclose(file);
                free(novo);
                return;
            }
            novo->registos->start = NULL;
            novo->prox = NULL;

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
            atual = lista_doentes->start;
            anterior = NULL;
        }
    }
    fclose(file);
}

//Funçao responsável por ler o ficheiro de registos dos doentes e carregar para a lista
void ler_ficheiro_registos(Doente_header *lista_doentes) {
    FILE *file = fopen("registos.txt", "r");
    char linha[MAX_TAM];
    int num_linhas = 0;

    if (file == NULL) {
        printf("Erro ao abrir o arquivo de registros.\n");
        exit(-1);
    }

    while (fgets(linha, sizeof(linha), file) != NULL) {
        num_linhas++;
    }

    if (num_linhas % 6 != 0) {
        printf("Erro: O número de linhas no ficheiro de registos não é valido.\n");
        fclose(file);
        exit(1);
    }
    rewind(file);
    Registo *novo_registo = NULL;
    Doente *atual_doente = lista_doentes->start;
    while (fgets(linha, sizeof(linha), file) != NULL) {
        if (strcmp(linha, "\n") == 0 || strcmp(linha, "\r\n") == 0) {
            continue;
        }

        novo_registo = (Registo *)malloc(sizeof(Registo));
        if (novo_registo == NULL) {
            printf("Erro ao alocar memória para o novo registro.\n");
            fclose(file);
            return;
        }
        int id_registo;
        if (sscanf(linha, "%d", &id_registo) == 1) {
            fgets(linha, sizeof(linha), file);
            sscanf(linha, "%d/%d/%d", &novo_registo->data_res.dia, &novo_registo->data_res.mes, &novo_registo->data_res.ano);

            fgets(linha, sizeof(linha), file);
            sscanf(linha, "%d", &novo_registo->tensao_max);

            fgets(linha, sizeof(linha), file);
            sscanf(linha, "%d", &novo_registo->tensao_min);

            if (!validar_tensao(novo_registo->tensao_max, novo_registo->tensao_min)) {
                printf("Erro: Tensão máxima menor ou igual à tensão mínima em algum dos registos.\n");
                fclose(file);
                free(novo_registo);
                exit(1);
            }
            fgets(linha, sizeof(linha), file);
            sscanf(linha, "%d", &novo_registo->peso);

            fgets(linha, sizeof(linha), file);
            sscanf(linha, "%d", &novo_registo->altura);

            novo_registo->prox = NULL;

            while (atual_doente != NULL && atual_doente->id_doente != id_registo) {
                atual_doente = atual_doente->prox;
            }
            if (atual_doente != NULL) {
                Registo *anterior = NULL;
                Registo *atual_registo = atual_doente->registos->start;

                while (atual_registo != NULL && novo_registo->tensao_max < atual_registo->tensao_max) {
                    anterior = atual_registo;
                    atual_registo = atual_registo->prox;
                }

                if (anterior == NULL) {
                    novo_registo->prox = atual_doente->registos->start;
                    atual_doente->registos->start = novo_registo;
                } else {
                    anterior->prox = novo_registo;
                    novo_registo->prox = atual_registo;
                }
            } else {
                printf("Erro: ID de paciente não encontrado na lista de doentes.\n");
                free(novo_registo);
            }
        }
        atual_doente = lista_doentes->start;
    }
    fclose(file);
}


//Funçao responsável por escrevee para o ficheiro de doentes
void escrever_ficheiro_doentes(Doente *novo){
    FILE *file = fopen("doentes.txt", "a");
    if (file == NULL) {
        printf("Erro ao abrir o arquivo para escrita.\n");
        return;
    }
    fprintf(file,"%d\n", novo->id_doente);
    fprintf(file,"%s\n", novo->nome);
    fprintf(file,"%d/%d/%d\n", novo->data_nasc.dia,novo->data_nasc.mes,novo->data_nasc.ano);
    fprintf(file,"%s\n", novo->CC);
    fprintf(file,"%d\n", novo->telemovel);
    fprintf(file,"%s\n", novo->email);
    fclose(file);
}

//Funçao responsável por escrevee para o ficheiro de registos dos doentes
void escrever_ficheiro_registos(Registo *novo, int id_novo_registo){
    FILE *file = fopen("registos.txt", "a");
    if (file == NULL) {
        printf("Erro ao abrir o arquivo para escrita.\n");
        return;
    }
    fprintf(file,"%d\n", id_novo_registo);
    fprintf(file,"%d/%d/%d\n", novo->data_res.dia,novo->data_res.mes,novo->data_res.ano);
    fprintf(file,"%d\n", novo->tensao_max);
    fprintf(file,"%d\n", novo->tensao_min);
    fprintf(file,"%d\n", novo->peso);
    fprintf(file,"%d\n", novo->altura);
    fclose(file);
}

//Funçao responsável por apagar um doente do ficheiro de doentes
void apagar_ficheiro_doentes(int id) {
    FILE *arquivo_leitura = fopen("doentes.txt", "r");
    FILE *arquivo_escrita = fopen("temp.txt", "w");
    int id_atual;
    char linha[20];
    int linha_atual = 1; 

    if (arquivo_leitura == NULL || arquivo_escrita == NULL) {
        printf("Erro ao abrir o arquivo.\n");
        exit(1);
    }
    while (fgets(linha, 20, arquivo_leitura) != NULL) {
        sscanf(linha, "%d", &id_atual);
        if (id_atual == id) {
            for (int i = 0; i < 5; i++) {
                if (fgets(linha, 20, arquivo_leitura) == NULL) {
                    printf("Erro: Formato de arquivo inválido.\n");
                    exit(1);
                }
                linha_atual++;
            }
        } else {
           
            fprintf(arquivo_escrita, "%s", linha);
        }
        linha_atual++;
    }

    fclose(arquivo_leitura);
    fclose(arquivo_escrita);

    remove("doentes.txt");
    rename("temp.txt", "doentes.txt");
}


//Funçao responsável por apagar os registos do doente a eliminar do ficheiro de registos
void apagar_ficheiro_registos(int id){
    FILE *arquivo_leitura = fopen("registos.txt", "r");
    FILE *arquivo_escrita = fopen("temp.txt", "w");
    int id_atual;
    char linha[20];
    int linha_atual = 1;  

    if (arquivo_leitura == NULL || arquivo_escrita == NULL) {
        printf("Erro ao abrir o arquivo.\n");
        exit(1);
    }

    while (fgets(linha, 20, arquivo_leitura) != NULL) {
        sscanf(linha, "%d", &id_atual);
        if (id_atual == id) {
            for (int i = 0; i < 5; i++) {
                if (fgets(linha, 20, arquivo_leitura) == NULL) {
                    printf("Erro: Formato de arquivo inválido.\n");
                    exit(1);
                }
                linha_atual++;
            }
        } else {
            fprintf(arquivo_escrita, "%s", linha);
        }
        linha_atual++;
    }
    fclose(arquivo_leitura);
    fclose(arquivo_escrita);

    remove("registos.txt");
    rename("temp.txt", "registos.txt");
}
