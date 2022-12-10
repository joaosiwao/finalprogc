#include "tabela_hash.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define TAM 15

/*Implementação das tad auxiliares para a reserva*/
struct data{
    int dia;
    int mes;
    int ano;
};

struct passageiro{
    int id;
    char *nome;
    char *endereco;
};

struct voo{
    int codigo;
    char *origem;
    char *destino;
};

//Implementação da reserva (tad da lista encadeada trechos)
struct reserva{
    int codigo;
    Data *data_viagem;
    Passageiro *passageiro;
    Voo *voo;
    Assento assento;
};

struct no{
    Reserva reserva;
    No *proximo;
};

struct trecho{ //Lista encadeada de Reservas.
    No *inicio;
    int tam;
};

//Tabela_hash definida globalmente.
Trecho *tabela_hash[TAM];

/*Funções|métodos auxiliares para manipular as tads da Reserva*/
void cadastrar_data(Reserva* reserva){
    Data* data = (Data*)malloc(sizeof(Data));

    printf("Informe o dia da viagem:");
    scanf("%i", &data->dia);
    fflush(stdin);

    printf("Informe o mes da viagem:");
    scanf("%i", &data->mes);
    fflush(stdin);
    
    printf("Informe o ano da viagem:");
    scanf("%i", &data->ano);
    fflush(stdin);

    reserva->data_viagem = data;
}

char *alocar_char(int tam){
    char *str = (char*)(malloc(sizeof(char)*tam));
    return str;
}

void cadastrar_passageiro(Reserva *reserva){
    Passageiro *passageiro = (Passageiro*)malloc(sizeof(Passageiro));
    passageiro->nome = alocar_char(50);
    passageiro->endereco = alocar_char(50);

    printf("Informe o id do passageiro: \n");
    scanf("%d", &passageiro->id);
    fflush(stdin);

    printf("Informe o nome do passageiro: \n");
    gets(passageiro->nome);
    fflush(stdin);

    printf("Informe o endereco do passageiro: \n");
    gets(passageiro->endereco);
    fflush(stdin);

    reserva->passageiro = passageiro;
}

void cadastrar_voo(Reserva *reserva){
    Voo *voo = (Voo*)malloc(sizeof(Voo));
    voo->origem = alocar_char(50);
    voo->destino = alocar_char(50);

    printf("Informe o codigo do voo: \n");
    scanf("%d", &voo->codigo);
    fflush(stdin);

    printf("Informe a origem do voo: \n");
    gets(voo->origem);
    fflush(stdin);

    printf("Informe a destino do voo: \n");
    gets(voo->destino);
    fflush(stdin);

    reserva->voo = voo;
}

void cadastrar_assento(Reserva *reserva){
    Assento novo_assento;
    Assento *assento_aux = (Assento*)malloc(sizeof(Assento));
    printf("Informe o num do assento 0 a 30: \n");
    scanf("%d", assento_aux);
    
    for(novo_assento = A0; novo_assento<=C9; novo_assento++){
        if(novo_assento == *assento_aux){
            printf("Assento: %d\n", novo_assento);
            reserva->assento = *assento_aux;
        }
    } 
}

void imprimir_data(Data *data){
    printf("\t  * Data: \n");
    printf("\t\t %i / %i / %i\n", data->dia, data->mes, data->ano);
}

void imprimir_passageiro(Passageiro *passageiro){
    printf("\t  * Pasageiro: \n");
    printf("\t\t nome: %s\n", passageiro->nome);
    printf("\t\t endereco: %s\n", passageiro->endereco);
    printf("\t\t id: %d\n", passageiro->id);
}

void imprimir_voo(Voo *voo){
    printf("\t  * Voo: \n");
    printf("\t\t codigo: %d\n", voo->codigo);
    printf("\t\t origem: %s\n", voo->origem);
    printf("\t\t destino: %s\n", voo->destino);
}

/*Funções|métodos para manipular a tad Reserva*/
Reserva criar_reserva(){ 
    Reserva nova_reserva;
    printf("Informe o codigo da reserva: \n");
    scanf("%d", &nova_reserva.codigo);
    
    cadastrar_data(&nova_reserva);
    cadastrar_passageiro(&nova_reserva);
    cadastrar_voo(&nova_reserva); 
    cadastrar_assento(&nova_reserva);
    
    return nova_reserva;
}

Reserva editar_reserva(int id_passageiro, int codigo_reserva){
    deletar_reserva_no_trecho(id_passageiro, codigo_reserva);
    Reserva nova_reserva;
    nova_reserva.codigo = codigo_reserva;

    cadastrar_data(&nova_reserva);
    cadastrar_passageiro(&nova_reserva);
    cadastrar_voo(&nova_reserva); 
    cadastrar_assento(&nova_reserva);
    
    return nova_reserva;
}

void imprimir_reserva(Reserva r){
    printf("\t<----------Reserva---------->\n");
    printf("\t * Codigo da reserva: %d\t\n", r.codigo);
    imprimir_data(r.data_viagem);
    imprimir_passageiro(r.passageiro);
    imprimir_voo(r.voo);
    printf("\t * Assento: %d\t\n", r.assento);
    printf("\t<--------------------------->\n");
}

//Cria uma lista de encadeada de Reservas (Trecho)
Trecho *criar_trecho(){
    Trecho *novo_trecho = (Trecho*)malloc(sizeof(Trecho));
    novo_trecho->inicio = NULL;
    novo_trecho->tam = 0;
    return novo_trecho;
}

//Insere uma reserva na lista encadeada de reservas (Trechos)
void inserir_reserva_no_trecho(Reserva reserva, Trecho *trecho){
    No *no = (No*) malloc(sizeof(No));
    no->reserva = reserva;
    no->proximo = trecho->inicio;
    trecho->inicio = no;
    trecho->tam++;
}

void deletar_reserva_no_trecho(int id, int codigo_reserva){
    int hash(int id_passageiro, int codigo_reserva);
    int indice = hash(id,codigo_reserva);
    No** atual = &tabela_hash[indice]->inicio;
    while((*atual) != NULL)
    {
        if ((*atual)->reserva.codigo == codigo_reserva)
        {
            No *no = *atual;
            *atual = (*atual)->proximo;
            free(no);
            tabela_hash[indice]->tam--;
            break;
        }
        atual = &(*atual)->proximo;
    }
}

No* buscar_reserva(int codigo, No *inicio){
    while(inicio != NULL){
        if(inicio->reserva.codigo == codigo)
            return inicio;
        else    
            inicio = inicio->proximo;
    }
    return NULL;
}

void imprimir_trecho(No *inicio){
    while(inicio != NULL) {
        imprimir_reserva(inicio->reserva);
        inicio = inicio->proximo;
    }
}

/*Funções|métodos para manipular a tabela hash*/
void inicializar_tabela_hash(){
    int i;
    for(i = 0; i < TAM; i++)
        tabela_hash[i] = criar_trecho();
}

//Função de hash
int hash(int id_passageiro, int codigo_reserva){
    return (id_passageiro+codigo_reserva) % TAM;
}

//Cria uma reserva e insere no trecho
void inserir_trecho_na_tabela(){
    Reserva nova_reserva = criar_reserva();
    int indice = hash(nova_reserva.passageiro->id, nova_reserva.codigo);
    inserir_reserva_no_trecho(nova_reserva, tabela_hash[indice]);
}

void editar_reserva_no_trecho(int id_passageiro, int codigo_reserva){
    Reserva nova_reserva = editar_reserva(id_passageiro, codigo_reserva);
    int indice = hash(nova_reserva.passageiro->id, nova_reserva.codigo);
    inserir_reserva_no_trecho(nova_reserva, tabela_hash[indice]);
}

//Busca por uma pessoa na tabela_hash.
Reserva *buscar_reserva_na_tabela_hash(int id_passageiro, int codigo_reserva){
    int indice = hash(id_passageiro, codigo_reserva);
    No *no = buscar_reserva(codigo_reserva, tabela_hash[indice]->inicio);
    if(no)
        return &no->reserva;
    else
        return NULL;
}

// imprimir tabela
void imprimir_tabela_hash(){
    int i;
    printf("\n---------------------VIAGENS-------------------------\n");
    for(i = 0; i < TAM; i++){
        printf("Trecho [%d] Reservas: %d\n", i, tabela_hash[i]->tam);
        imprimir_trecho(tabela_hash[i]->inicio);
    }
    printf("---------------------FIM TABELA-----------------------\n\n");
}

int main() {
    int op, cod, id;
    Reserva *r;

    inicializar_tabela_hash();

    do {
        printf("----------------------- MENU -------------------------\n");
        printf("\n\t\t0 - Sair do Menu\n");
        printf("\t\t1 - Inserir Reserva\n"); 
        printf("\t\t2 - Editar Reserva\n"); 
        printf("\t\t3 - Deletar Reserva\n"); 
        printf("\t\t4 - Buscar Reserva\n");
        printf("\t\t5 - Imprimir tabela\n\t\tOp: ");
        scanf("%d", &op);
        
        switch(op){
        case 0:
            printf("\t\tSaindo...\n");
            break;
        
        case 1:
            inserir_trecho_na_tabela();
            break;
        
        case 2:
            printf("Informe o codigo da reserva: \n");
            scanf("%d", &cod);
            printf("Informe o id do passageiro: \n");
            scanf("%d", &id);
            editar_reserva_no_trecho(id, cod);
            break;

        case 3:
            printf("Informe o codigo da reserva: \n");
            scanf("%d", &cod);
            printf("Informe o id do passageiro: \n");
            scanf("%d", &id);
            deletar_reserva_no_trecho(id, cod);
            break;
        
        case 4:
            printf("Informe o codigo da reserva: \n");
            scanf("%d", &cod);
            printf("Informe o id do passageiro: \n");
            scanf("%d", &id);

            r = buscar_reserva_na_tabela_hash(id, cod);
            if(r) {
                printf("\t Reserva encontrada!\n");
                printf("\t  * Codigo: %d\n", r->codigo);
                imprimir_data(r->data_viagem);
                imprimir_passageiro(r->passageiro);
                imprimir_voo(r->voo);
                printf("Assento: %d\n", r->assento);
            } else
                printf("\tReserva nao encontrada!\n");
            break;
        
        case 5:
            imprimir_tabela_hash();
            break;
        
        default:
            printf("Opcao invalida!\n");
        }
        printf("----------------------FIM MENU------------------------\n\n");
    } while(op != 0);

    return 0;
}
