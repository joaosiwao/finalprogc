/*Structs auxiliares para compor a reserva*/
typedef struct data Data;
typedef struct passageiro Passageiro;
typedef struct voo Voo;

/*Enum para mapear os assentos*/
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

/*Principais structs para tabela_hash*/
typedef struct reserva Reserva;
typedef struct no No; //no para reserva
typedef struct trecho Trecho; 

/*Funções*/
void cadastrar_data(Reserva* reserva);
char *alocar_char(int tam);
void cadastrar_passageiro(Reserva *reserva);
void cadastrar_voo(Reserva *reserva);
void cadastrar_assento(Reserva *reserva);
void imprimir_data(Data *data);
void imprimir_passageiro(Passageiro *passageiro);
void imprimir_voo(Voo *voo);
Reserva criar_reserva();
Reserva editar_reserva(int id_passageiro, int codigo_reserva);
void imprimir_reserva(Reserva r);
Trecho *criar_trecho();
void inserir_reserva_no_trecho(Reserva reserva, Trecho *trecho);
void deletar_reserva_no_trecho(int id, int codigo_reserva);
No* buscar_reserva(int codigo, No *inicio);
void imprimir_trecho(No *inicio);
void inicializar_tabela_hash();
int hash(int id_passageiro, int codigo_reserva);
void inserir_trecho_na_tabela();
void editar_reserva_no_trecho(int id_passageiro, int codigo_reserva);
Reserva *buscar_reserva_na_tabela_hash(int id_passageiro, int codigo_reserva);
void imprimir_tabela_hash();
