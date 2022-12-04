#include "TravelBooking.h"
#include <stdio.h>
#include <stdlib.h>

struct data {
  int dia;
  int mes;
  int ano;
};

struct voo {
  int codigo;
  char *origem;
  char *destino;
};

struct no_voo {
  Voo *voo;
  struct no_voo *proximo;
};

struct lista_voo {
  struct no_voo *primeiro;
};

struct passageiro {
  int id;
  char *nome;
  char *endereco;
};

struct no_passageiro {
  Passageiro *passageiro;
  struct no_passageiro *proximo;
};

struct lista_passageiro {
  struct no_passageiro *primeiro;
};

struct reserva{
  int codigo;
  Data *data_viagem;
  Passageiro *passageiro;
  Voo *voo;
  Assento assento;
};

struct agenda {
  Reserva *reserva;
  Agenda *esq;
  Agenda *dir;
};

struct trecho {
  Reserva *reserva;
  struct trecho *proximo;
};

struct viagem {
  struct trecho *trechos;
};

struct tabela_viagem {
  int tamanho;
  Viagem *tabela_hash;
};


//"agenda" é como nó -> struct q contem reserva, esq e dir


//retorna 0 caso a data nova seja menor que a antiga (esquerda) e 1 caso a data nova seja maior ou igual a antiga (direita).
int *checaData(Data Nova,Data Ant){
  int diaN, mesN, anoN ;
  int diaA, mesA, anoA ;

  diaN = Nova.dia ;
  mesN = Nova.mes ;
  anoN = Nova.ano ;

  diaA = Ant.dia ;
  mesA = Ant.mes ;
  anoA = Ant.ano ; 

  if (anoA > anoN){
    return 0;
  }
  if (anoA == anoN){
    if (mesA > mesN){
      return 0;
    }
    else if (mesA == mesN){
      if (diaA > diaN){
        return 0;
      }
  }
  }
  else{
    return 1;
  }
  return NULL;
}

Data hoje, *aux;
Data aux;


/* Cria ABB */
Agenda *IniciaABB(Reserva primeira){

//Dados Invalidos: (não sei se a questao aux e *aux esta certa)
aux = primeira.data_viagem;
if (checaData(*aux, hoje) == 0){
  return NULL; //data invalida (<hoje)
}

Reserva *auxiliar;
*auxiliar = primeira;

//Happy path: (completamente perdido na questao dos ponteiros)
  Agenda *raiz = (Agenda *)malloc(sizeof(Agenda));
  raiz->reserva = auxiliar;
  raiz->esq = (Agenda *)malloc(sizeof(Agenda));
  raiz->dir = (Agenda *)malloc(sizeof(Agenda));
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

    checagemData  = checaData(busca,inicial);


    if (checagemData == 0) {
      return BuscaAgenda(raiz->esq, busca);
    } else {
      return BuscaAgenda(raiz->dir, busca);
    }
  }

  return NULL;
}




/* Adiciona uma agenda à esquerda(caso for menor) ou à direita(caso for maior ou igual) do no raiz. Retorna a raiz da árvore
 * resultante ou NULL raiz e o nó sejam NULL */

Agenda *InsereAgenda(Agenda *raiz, Agenda *nova) {
  if (raiz == NULL && nova == NULL) {
    return NULL;
  } else if (raiz == NULL) {
    return nova;
  }

  if (raiz->reserva->data_viagem < nova->reserva->data_viagem) {
      raiz->esq = InsereAgenda(raiz->esq, nova);
  } 
    
   else {
      raiz->dir = InsereAgenda(raiz->dir, nova);
  }
  return raiz;

  }


/*Realiza uma busca linear pelo código de reserva*/

Agenda *BuscaCodigo(Agenda *raiz, int reserva){
  if (raiz->reserva->codigo == reserva){
    return raiz;
  }
  
  if(raiz != NULL){
        BuscaCodigo(raiz->esq,reserva);
        BuscaCodigo(raiz->dir,reserva);
    }
}

/*Remove Agenda com um dado código de reserva*/




/*Busca por (i) identificador do passageiro e código do voo*/

Agenda *BuscaI(Agenda *raiz, Passageiro passageiro, int codigo){
  Passageiro *aux;
  *aux = passageiro;
  if (raiz->reserva->codigo == codigo && raiz->reserva->passageiro == aux){
    return raiz;
  }
  
  if(raiz != NULL){
        BuscaI(raiz->esq,passageiro,codigo);
        BuscaI(raiz->dir,passageiro,codigo);
    }
}

/*Busca por (ii) identificador do passageiro e data da viagem.*/

Agenda *BuscaII(Agenda *raiz, Passageiro passageiro, Data data){
  Passageiro *aux;
  *aux = passageiro;
  
  Data *auxi;
  *auxi = data;

  if (raiz->reserva->data_viagem == auxi && raiz->reserva->passageiro == aux){
    return raiz;
  }
  
  if(raiz != NULL){
        BuscaII(raiz->esq,passageiro,data);
        BuscaII(raiz->dir,passageiro,data);
    }
}



/*Edita a reserva*/

Agenda *Edita(Agenda *raiz, int codEditar){
  Agenda *edita;
  edita = BuscaCodigo(raiz,codEditar);
  }



//int arvore;


/*

  isso provavelmente vai estar em outro arquivo

int main(){
    while (1){ //Base do programa

        if (arvore == NULL){ // Caso não exista arvore ainda(ainda n sei como arrumar essa ideia de arvore)
            Reserva raiz;
        };

        //Menu de operações:
        int op;
        printf("Digite o número relacionado a operação desejada: ");
        printf("Criar nova reserva: 1");
        
        scanf("%d",&op);

        if (op = 1){
            No *abb_insere_no(No *raiz, No *no);
        };
        


    }
}*/