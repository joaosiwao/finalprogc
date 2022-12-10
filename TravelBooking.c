#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "tabela_hash.h"

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
    struct Agenda *pai;
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
Agenda *IniciaABB(){

Reserva *primeira;
Reserva *prim;
primeira = PreencheReserva();
prim = primeira;



Data *aux = prim->data_viagem;


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
Agenda *BuscaAnterior(Agenda *raiz, int reserva){
    if (raiz == NULL){
        return NULL;
    }
    if (raiz->esq->reserva->codigo-> == reserva){
        return raiz;
    }
    if (raiz->dir->reserva->codigo-> == reserva){
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
/*Agenda *removeCodigo(Agenda *raiz, int codigo){
    Agenda noRemove = BuscaCodigo(*raiz, codigo);
    Agenda antRemove = BuscaAnterior(*raiz, codigo);
    if(noRemove->esq == NULL && noRemove.dir == NULL){
        *Faz o nó anterior apontar para NULL seja onde antes apontava pra ele
    }
    if(noRemove->esq == NULL){
    }
    if(noRemove->dir == NULL){
    }

        Agenda noMinimo = BuscaMinimo(*noRemove);

}
Agenda *BuscaMinimo(Agenda *raiz){
    *Vai ao máximo a esq do Nó enquanto for diferente de NULL.

    Agenda aux = raiz->dir;

    while(aux->esq != Null){
        aux = aux->esq;
    }

}
void Transplante(Agenda *raiz){
     *I-Identifica nó a se remover
     *II- Percorre com BuscaMinimo(noRemover->dir)
     *III- Armazena em uma variável e substitui por NULL na abb
     *IV- Substitui o noRemove pela variável
     *V- Retorna abb alterada
}*/


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

    printf("Qual campo deseja editar? \n");
    printf("1- Data \n");
    printf("2- Passageiro \n");
    printf("3- Assento \n");
    printf("4- Voo \n");
    int opc;
    scanf("%d",&opc);

    if (opc = 1){
        edita->reserva->data_viagem = cadastrar_data(edita->reserva);
    }
    if (opc = 2){
        edita->reserva->passageiro = cadastrar_passageiro(edita->reserva);
    }
    if (opc = 3){
        edita->reserva->assento = cadastrar_assento(edita->reserva);
    }
    if (opc = 4){
        edita->reserva->voo = cadastrar_voo(edita->reserva);
    }
    removeCodigo(raiz,codEditar);
    InsereAgenda(raiz, edita);
}

/*Preenche os dados de uma nova Reserva*/

Reserva *PreencheReserva(){
    Reserva *fill;

    cadastrar_data(fill);
    cadastrar_passageiro(fill);
    cadastrar_assento(fill);
    cadastrar_voo(fill);

    return fill;
}


/*Funções|métodos auxiliares para manipular as tads da Reserva*/
Reserva *cadastrar_data(Reserva* reserva){
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
    return reserva;
}

char *alocar_char(int tam){
    char *str = (char*)(malloc(sizeof(char)*tam));
    return str;
}

Reserva *cadastrar_passageiro(Reserva *reserva){
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
    return reserva;
}

Reserva *cadastrar_voo(Reserva *reserva){
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
    return reserva;
}

Reserva *cadastrar_assento(Reserva *reserva){
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
    return reserva;
}