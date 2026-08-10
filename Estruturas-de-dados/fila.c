#include <stdio.h>
#include <stdlib.h>

typedef struct {
    char* nome;
    int idade;
} Pessoa;

#define MAX 5

typedef struct {
    Pessoa itens[MAX];
    int inicio;
    int final;
    int total;
} Fila;

// Inicializa somente as variáveis de controle
void inicializarFila(Fila *f) {
    f->inicio = 0;
    f->final = 0;
    f->total = 0;
}

int filaCheia(Fila *f) {
    return f->total == MAX;
}

int filaVazia(Fila *f) {
    return f->total == 0;
}

void inserir(Fila *f, Pessoa p) {
    if(filaCheia(f)) {
        printf("Fila cheia. Não é possível adicionar o item.\n");
        return;
    }

    f->itens[f->final] = p;
    f->final = (f->final + 1) % MAX;
    f->total++;
}

void remover(Fila *f, Pessoa *p) {
    if(filaVazia(f)) {
        printf("Não é possível remover o item.\n");
        return;
    }

    *p = f->itens[f->inicio]; // Devolve o item removido ao código chamador
    f->inicio = (f->inicio + 1) % MAX;
    f->total--;
}

void mostrarFila(Fila *f) {
    printf("Fila: ");
    for(int i = 0, idx = f->inicio; i<f->total; i++, idx = (idx+1) % MAX) {
        printf("[%s, %d] ", f->itens[idx].nome, f->itens[idx].idade);
    }
    printf("\n");
}

int main()
{
    Fila f;
    inicializarFila(&f); // Inicializa a estrutura f

    // Criar os elementos que vão ser adicionados na FILA!
    Pessoa p1 = {"Jhon Doe", 36};
    Pessoa p2 = {"Mr Robot", 26};
    Pessoa p3 = {"Pessoa 3", 32};
    Pessoa p4 = {"Pessoa 4", 46};
    Pessoa p5 = {"Pessoa 5", 56};
    Pessoa p6 = {"Pessoa 6", 56};

    // Insere as pessoas na FILA!
    inserir(&f, p1);
    inserir(&f, p2);
    inserir(&f, p3);
    inserir(&f, p4);
    inserir(&f, p5);

    // Tenta inserir mais um elemento
    inserir(&f, p6);

    mostrarFila(&f); // Imprime a fila

    // Remove um elemento da FILA (FIFO)!
    Pessoa removida; // Pois é uma FIFO, primeiro entra, primeiro sai
    remover(&f, &removida); // se colocar &p1 ocorre segmentation fault

    printf("Pessoa removida: %s\n", removida.nome);

    mostrarFila(&f); // Mostra após remover!
    return 0;
}
