#include <stdio.h>

// Desafio Tetris Stack
// Tema 3 - Integração de Fila e Pilha
// Este código inicial serve como base para o desenvolvimento do sistema de controle de peças.
// Use as instruções de cada nível para desenvolver o desafio.

// 🧩 Nível Novato: definição de tipos e fila
typedef struct Peca
{
    char tipo; /* piece type, e.g. 'I','O','T','S','Z','J','L' */
    int id;    /* sequential identifier */
} Peca;

#define MAX_FILA 5
typedef struct Fila
{
    Peca pecas[MAX_FILA];
    int inicio;
    int fim;
    int quantidade;
} Fila;

/* Funções da fila */
void inicializarFila(Fila *fila) {
    fila->inicio = 0;
    fila->fim = 0;
    fila->quantidade = 0;
}
void enquete(Fila *fila, Peca peca) {
    if (fila->quantidade < MAX_FILA) {
        fila->pecas[fila->fim] = peca;
        fila->fim = (fila->fim + 1) % MAX_FILA;
        fila->quantidade++;
    }
}
Peca dequeue(Fila *fila) {
    Peca pecaRemovida = {0};
    if (fila->quantidade > 0) {
        pecaRemovida = fila->pecas[fila->inicio];
        fila->inicio = (fila->inicio + 1) % MAX_FILA;
        fila->quantidade--;
    }
    return pecaRemovida;
}
int filaCheia(Fila *fila) {
    return fila->quantidade == MAX_FILA;
}
int filaVazia(Fila *fila) {
    return fila->quantidade == 0;
}
Peca gerarPeca(int id) {
    Peca novaPeca;
    char tipos[] = {'I', 'O', 'T', 'S', 'Z', 'J', 'L'};
    novaPeca.tipo = tipos[id % 7]; // Simplesmente cicla pelos tipos
    novaPeca.id = id;
    return novaPeca;
}
void mostrarFila(Fila *fila) {
    printf("Fila de Peças:\n");
    for (int i = 0; i < fila->quantidade; i++) {
        int index = (fila->inicio + i) % MAX_FILA;
        printf("Peça ID: %d, Tipo: %c\n", fila->pecas[index].id, fila->pecas[index].tipo);
    }
}

/* 🧠 Nível Aventureiro: pilha de reserva */
#define MAX_PILHA 3
typedef struct Pilha
{
    Peca pecas[MAX_PILHA];
    int topo;
} Pilha;

void inicializarPilha(Pilha *pilha) {
    pilha->topo = -1;
}
void push(Pilha *pilha, Peca peca) {
    if (pilha->topo < MAX_PILHA - 1) {
        pilha->pecas[++pilha->topo] = peca;
    }
}
Peca pop(Pilha *pilha) {
    Peca pecaRemovida = {0};
    if (pilha->topo >= 0) {
        pecaRemovida = pilha->pecas[pilha->topo--];
    }
    return pecaRemovida;
}
int pilhaCheia(Pilha *pilha) {
    return pilha->topo == MAX_PILHA - 1;
}
int pilhaVazia(Pilha *pilha) {
    return pilha->topo == -1;
}
void mostrarPilha(Pilha *pilha) {
    printf("Pilha de Reserva:\n");
    if (pilha->topo < 0) {
        printf("(vazia)\n");
        return;
    }
    for (int i = pilha->topo; i >= 0; i--) {
        printf("Peça ID: %d, Tipo: %c\n", pilha->pecas[i].id, pilha->pecas[i].tipo);
    }
}

/* Função principal contendo apenas a lógica de execução */
int main(void) {
    Fila fila;
    Pilha pilha;
    int idCounter = 0;
    int opcao = -1;

    inicializarFila(&fila);
    inicializarPilha(&pilha);

    /* Inicializa a fila com 5 peças */
    for (int i = 0; i < MAX_FILA; i++) {
        Peca p = gerarPeca(idCounter++);
        enquete(&fila, p);
    }

    /* Loop principal do menu com todas as opções previstas */
    do {
        printf("\nMenu:\n");
        printf("1 - Jogar peça (remover da frente)\n");
        printf("2 - Enviar peça da fila para a reserva (pilha)\n");
        printf("3 - Usar peça da reserva (remover do topo da pilha)\n");
        printf("4 - Trocar peça da frente da fila com o topo da pilha\n");
        printf("5 - Trocar 3 primeiros da fila com os 3 da pilha\n");
        printf("0 - Sair\n");
        printf("Escolha uma opção: ");
        if (scanf("%d", &opcao) != 1) {
            /* entrada inválida */
            opcao = 0;
            break;
        }

        if (opcao == 1) {
            if (!filaVazia(&fila)) {
                Peca jogada = dequeue(&fila);
                printf("Jogou a peça ID: %d, Tipo: %c\n", jogada.id, jogada.tipo);
                /* repõe a fila para manter sempre MAX_FILA peças */
                Peca nova = gerarPeca(idCounter++);
                enquete(&fila, nova);
            } else {
                printf("Fila vazia.\n");
            }
            mostrarFila(&fila);
            mostrarPilha(&pilha);
        } else if (opcao == 2) {
            if (!filaVazia(&fila) && !pilhaCheia(&pilha)) {
                Peca p = dequeue(&fila);
                push(&pilha, p);
                printf("Enviou para reserva a peça ID: %d, Tipo: %c\n", p.id, p.tipo);
                /* repõe a fila */
                Peca nova = gerarPeca(idCounter++);
                enquete(&fila, nova);
            } else {
                printf("Não é possível enviar para reserva (fila vazia ou pilha cheia).\n");
            }
            mostrarFila(&fila);
            mostrarPilha(&pilha);
        } else if (opcao == 3) {
            if (!pilhaVazia(&pilha)) {
                Peca p = pop(&pilha);
                printf("Usou a peça da reserva ID: %d, Tipo: %c\n", p.id, p.tipo);
                /* Ao usar reserva, normalmente a peça não volta para a fila; não repondo aqui */
            } else {
                printf("Reserva vazia.\n");
            }
            mostrarFila(&fila);
            mostrarPilha(&pilha);
        } else if (opcao == 4) {
            if (!filaVazia(&fila) && !pilhaVazia(&pilha)) {
                int idxFila = fila.inicio;
                Peca tmp = fila.pecas[idxFila];
                fila.pecas[idxFila] = pilha.pecas[pilha.topo];
                pilha.pecas[pilha.topo] = tmp;
                printf("Trocou frente da fila com topo da pilha.\n");
            } else {
                printf("Troca inválida: verifique se fila e pilha têm peças suficientes.\n");
            }
            mostrarFila(&fila);
            mostrarPilha(&pilha);
        } else if (opcao == 5) {
            if (pilha.topo == MAX_PILHA - 1 && fila.quantidade >= 3) {
                /* troca os 3 primeiros da fila (na ordem da frente) com os 3 da pilha (do topo para baixo) */
                for (int i = 0; i < 3; i++) {
                    int idxFila = (fila.inicio + i) % MAX_FILA;
                    int idxPilha = pilha.topo - i;
                    Peca tmp = fila.pecas[idxFila];
                    fila.pecas[idxFila] = pilha.pecas[idxPilha];
                    pilha.pecas[idxPilha] = tmp;
                }
                printf("Trocou os 3 primeiros da fila com as 3 peças da pilha.\n");
            } else {
                printf("Troca inválida: pilha precisa ter exatamente 3 peças e fila ao menos 3.\n");
            }
            mostrarFila(&fila);
            mostrarPilha(&pilha);
        } else if (opcao == 0) {
            printf("Saindo...\n");
        } else {
            printf("Opção inválida.\n");
        }

    } while (opcao != 0);

    return 0;
}


 


