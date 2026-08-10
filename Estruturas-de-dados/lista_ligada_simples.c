#include <stdio.h>
#include <stdlib.h>

// Listas circular encadeadas simples!
// o último aponta pro primeiro
struct No {
    int dado;
    struct No* proximo;
};

int main()
{
    struct No* no1 = (struct No*) malloc(sizeof(struct No));
    struct No* no2 = (struct No*) malloc(sizeof(struct No));
    struct No* no3 = (struct No*) malloc(sizeof(struct No));

    if(no1 == NULL) {
        return 1;
    } else if(no2 == NULL) {
        return 1;
    } else if(no3 == NULL) {
        return 1;
    } else {
        printf("Sem valores vazios, prosseguindo...\n");
    }

    no1->dado = 10;
    no1->proximo = no2;

    no2->dado = 20;
    no2->proximo = no3;

    no3->dado = 30;
    // O último nó aponta pro primeiro
    no3->proximo = no1;

    // Percorrer a lista
    struct No* no0 = no1;
    
    if(no0 == NULL) { return 1; }

    while(no0 != NULL)
    {
        printf("%d\n", no0->dado);
        no0 = no0->proximo;
        // Condição para quebrar looping se não fica rodando infinitamente
        if(no0->dado == no1->dado) {
            printf("%d\n", no0->dado);
            break;
        }
    }

    free(no1);
    free(no2);
    free(no3);

    printf("Memória liberada\n");

    return 0;
};
