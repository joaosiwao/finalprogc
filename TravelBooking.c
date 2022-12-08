#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct Data {
    int dia;
    int mes;
    int ano;
} Data;

typedef struct Voo {
    int codigo;
    char *origem;
    char *destino;
} Voo;

typedef struct No_voo {
    Voo *voo;
    struct No_voo *proximo;
} No_voo;

typedef struct Lista_voo {
    No_voo *primeiro;
} Lista_voo;

typedef struct Passageiro {
    int id;
    char *nome;
    char *endereco;

} Passageiro;

/*bool datas_iguais(struct Data primeiro, struct Data segundo) {
    if (primeiro.ano == segundo.ano && primeiro.mes == segundo.mes && primeiro.dia == segundo.dia) {
        return 1;
    }
    return 0;
}

bool passageiros_iguais(struct Passageiro primeiro, struct Passageiro segundo) {
    if (primeiro.id == segundo.id && strcmp(primeiro.nome, segundo.nome) &&
        strcmp(primeiro.endereco, segundo.endereco)) {
        return 1;
    }
    return 0;
}
*/
typedef struct No_passageiro {
    Passageiro *passageiro;
    struct No_passageiro *proximo;
} No_passageiro;

typedef struct Lista_passageiro {
    No_passageiro *primeiro;
} Lista_passageiro;

enum cod_assento {
    A0, B0, C0,
    A1, B1, C1,
    A2, B2, C2,
    A3, B3, C3,
    A4, B4, C4,
    A5, B5, C5,
    A6, B6, C6,
    A7, B7, C7,
    A8, B8, C8,
    A9, B9, C9
};

typedef enum cod_assento Assento;


typedef struct Reserva {
    int codigo;
    Data *data_viagem;
    Passageiro *passageiro;
    Voo *voo;
    Assento assento;
} Reserva;

typedef struct Agenda {
    Reserva *reserva;
    struct Agenda *esq;
    struct Agenda *dir;
} Agenda;

typedef struct Trecho {
    Reserva *reserva;
    struct Trecho *proximo;
} Trecho;

typedef struct Viagem {
    Trecho *trechos;
} Viagem;

typedef struct Tabela_viagem {
    int tamanho;
    Viagem *tabela_hash;
} Tabela_viagem;


//"agenda" é como nó -> struct q contem reserva, esq e dir


//retorna 0 caso a data nova seja menor que a antiga (esquerda) e 1 caso a data nova seja maior ou igual a antiga (direita).
int checaData(Data anterior, Data nova){

    if (anterior.ano < nova.ano){
        return 1;
    }
    if (anterior.ano == nova.ano){
        if (anterior.mes < nova.mes){
            return 1;
        }
        if (anterior.mes == nova.mes && anterior.dia <= nova.dia){
            return 1;
        }
    }
    return 0;
}


Data hoje;

/* Cria ABB */
Agenda *IniciaABB(Reserva primeira){

Data *aux = primeira.data_viagem;
Reserva *prim;
Reserva prim = primeira;


    //Dados Invalidos: (Data da viagem < Data de hoje)
    if (checaData(hoje, *aux) == 0){
        return NULL;
    }

//Happy path:
    Agenda *raiz = (Agenda *)malloc(sizeof(Agenda));
    raiz->reserva = prim;
    raiz->esq = NULL;
    raiz->dir = NULL;
    return raiz;
}



/* Procura a agenda pela data. Retorna a agenda caso a busca obtenha sucesso ou NULL
 * em caso contrário. */


Agenda *BuscaAgenda(Agenda *raiz, Data busca) { 
    Data *aux;
    *aux = busca;

    if (raiz != NULL) {
        if (raiz->reserva->data_viagem == aux){
            return raiz;
        }

        int checagemData;
        Data inicial;
        inicial = *raiz->reserva->data_viagem;

        checagemData = checaData(busca,inicial);


        if (checagemData == 0) {
            return BuscaAgenda(raiz->esq, busca);
        } else {
            return BuscaAgenda(raiz->dir, busca);
        }
    }

    return NULL;
}

int BuscaInd(Agenda *raiz, int codigo);

int BuscaVoo(Agenda *raiz, int voo);

int *BuscaRes(Agenda *raiz, int codigo);

int *BuscaDeR(Agenda *raiz, int id,Data nova);

/* Adiciona uma agenda à esquerda(caso for menor) ou à direita(caso for maior ou igual) do no raiz. Retorna a raiz da árvore
 * resultante ou NULL raiz e o nó sejam NULL */

Agenda *InsereAgenda(Agenda *raiz, Agenda *nova) {
    
    // impede de criar uma reserva com codigo de voo ou de reserva repetida
    Data *auxiliar;
    auxiliar = nova->reserva->data_viagem;


    int restricao, restricao2, restricao3, restricao4;
    restricao = BuscaInd(raiz,nova->reserva->passageiro->id);
    restricao2 = BuscaVoo(raiz,nova->reserva->voo->codigo);
    restricao3 = BuscaRes(raiz,nova->reserva->codigo);
    restricao4 = BuscaDeR(raiz,nova->reserva->passageiro->id,*auxiliar);

    if (restricao == 1){
        return NULL;
    }

    if (restricao2 == 1){
        return NULL;
    }

    if (restricao3 == 1){
       return NULL;
    }

    if (restricao4 == 1){
       return NULL;
    }

    if (raiz == NULL) {
        return nova;
    }
    // Insere a agenda
    Data *aux1, *aux2;
    aux1 = raiz->reserva->data_viagem;
    aux2 = nova->reserva->data_viagem;

    Data aux3, aux4;
    aux3 = *aux1;
    aux4 = *aux2;

    if (checaData(aux3, aux4) == 1){
        raiz->dir = InsereAgenda(raiz->dir, nova);
    } else {
        raiz->esq = InsereAgenda(raiz->esq, nova);
    }
    return raiz;

}


/*Realiza uma busca linear pelo código de reserva*/

Agenda *BuscaCodigo(Agenda *raiz, int reserva){
    if (raiz == NULL){
        return NULL;
    }
    if (raiz->reserva->codigo == reserva){
        return raiz;
    }
    Agenda *esq = BuscaCodigo(raiz->esq,reserva);
    Agenda *dir = BuscaCodigo(raiz->dir,reserva);
    if(esq != NULL){
        return esq;
    }
    return dir;
}

/*Remove Agenda com um dado código de reserva*/
/*bool RemoverAgenda() {
    if (raiz == NULL) {
        return NULL;
    }
    if (raiz->reserva->codigo == reserva) {
        return raiz;
    }
    Agenda *esq = BuscaCodigo(raiz->esq, reserva);
    Agenda *dir = BuscaCodigo(raiz->dir, reserva);
    if (esq != NULL) {
        return esq;
    }
    return dir;
}
*/

/*Busca por (i) identificador do passageiro e código do voo*/

Agenda *BuscaI(Agenda *raiz, Passageiro passageiro, int codigo){


Passageiro *passageiroaux;
*passageiroaux = passageiro;


    if (raiz == NULL){
        return NULL;
    }
    if (raiz->reserva->codigo == codigo && raiz->reserva->passageiro == passageiroaux){
        return raiz;
    }
    Agenda *esq = BuscaI(raiz->esq,passageiro, codigo);
    Agenda *dir = BuscaI(raiz->dir,passageiro, codigo);
    if(esq != NULL){
        return esq;
    }
    return dir;
}

/*Busca por (ii) identificador do passageiro e data da viagem.*/

Agenda *BuscaII(Agenda *raiz, Passageiro passageiro, Data data){

Passageiro *passageiroaux;
*passageiroaux = passageiro;

Data *dataaux;
*dataaux = data;


    if (raiz == NULL){
        return NULL;
    }
    if (raiz->reserva->data_viagem == dataaux && raiz->reserva->passageiro == passageiroaux){
        return raiz;
    }
    Agenda *esq = BuscaII(raiz->esq,passageiro,data);
    Agenda *dir = BuscaII(raiz->dir,passageiro,data);
    if(esq != NULL){
        return esq;
    }
    return dir;
}

/*Busca por identificador do passageiro*/

int *BuscaInd(Agenda *raiz, int codigo){

    if (raiz->reserva->passageiro->id == codigo){
        return 1;
    }

    if(raiz != NULL){
        BuscaInd(raiz->esq,codigo);
        BuscaInd(raiz->dir,codigo);
    }
}

/*Busca por identificador de reserva*/

int *BuscaRes(Agenda *raiz, int codigo){

    if (raiz->reserva->codigo == codigo){
        return 1;
    }

    if(raiz != NULL){
        BuscaRes(raiz->esq,codigo);
        BuscaRes(raiz->dir,codigo);
    }
}

/*Busca por identificador do voo*/

int *BuscaVoo(Agenda *raiz, int voo){

    if (raiz->reserva->voo->codigo == voo){
        return 1;
    }

    if(raiz != NULL){
        BuscaVoo(raiz->esq,voo);
        BuscaVoo(raiz->dir,voo);
    }
}

int *BuscaDeR(Agenda *raiz, int id,Data nova){

//Busca agenda editada
    Data *aux;
    *aux = nova;

    if (raiz != NULL) {
        if (raiz->reserva->data_viagem == aux){
            if (raiz->reserva->passageiro->id == id){
                return 1;
            }
        }

        int checagemData;
        Data inicial;
        inicial = *raiz->reserva->data_viagem;

        checagemData  = checaData(nova,inicial);


        if (checagemData == 0) {
            return BuscaDer(raiz->esq,id, nova);
        } else {
            return BuscaDer(raiz->dir,id, nova);
        }
    }

    return NULL;


}






/*Edita a reserva*/

Agenda *Edita(Agenda *raiz, int codEditar){
    Agenda *edita;
    edita = BuscaCodigo(raiz,codEditar);
    removeCodigo(raiz,codEditar);

    Data *dataNova;
    int ano, mes, dia;


    printf("Digite a data no formato ano,mes,dia");
    //scanfs

    dataNova->ano = ano;
    dataNova->mes = mes;
    dataNova->dia = dia;

    edita->reserva->passageiro; //esperar tabela hash

    edita->reserva->voo; //esperar tabela hash


    edita->reserva->data_viagem = dataNova;
    edita->reserva->assento; //esperar tabela hash

    //codigo permanece o mesmo


    InsereAgenda(raiz, edita);
}
