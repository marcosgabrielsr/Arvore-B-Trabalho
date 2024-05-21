//====== INFORMALÇÕES IMPORTANTES ======//
//
//

//====== Incluindo bibliotecas
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//====== Definindo constantes
#define D 2
#define M 2*D
#define MAX_SIZE_NAME 13

//====== Criando structs
struct no{
    int chaves[M];
    struct no * filhos[M + 1];
};

//====== Declarando protótipos das funções
//- Função que imprime o menu na tela
void menu();
//- Função de busca em árvores B
void buscaB(int x, struct no * r, struct no* pt, int* f, int* g);

int main() {
    struct no* raiz = NULL;

    raiz->chaves[0] = 50;

    menu();
    return 0;
}

//- Função que imprime o menu na tela
void menu() {
    int opt, run = 1;

    do {
        printf("\n// ----- // ----- // ÁRVORE B // ----- // ----- //\n");
        printf("[1] - Buscar\n");
        printf("[2] - Inserir\n");
        printf("[3] - Remover\n");
        printf("[9] - Finalizar\n");

        printf("Opção escolhida: ");
        scanf("%d", &opt);

        switch(opt) {
            case 1:
                printf("Buscar\n");
            break;
        
            case 9:
                run = 0;
                printf("Saindo...\n");
            break;

            default:
                printf("Opção inválida. Tente novamente...\n");
            break;
        }
    } while(run == 1);
}

//- Função de busca em árvores B
void buscaB(int x, struct no * r, struct no* pt, int* f, int* g) {
    struct no * p = r;
    int i = 0;
    *f = *g = 0;

    while(p != NULL) {
        pt = p;
        i = 0;
        *g = 0;

        while(i < M) {
            if(x == p->chaves[i]) {
                *f = 1;
                return;
            } else if(x > p->chaves[i]) {
                if(i == M - 1){
                    p = p->filhos[4];
                    i = M;
                }
                else
                    i += 1;
            } else {
                p = p->filhos[i];
                i = M;
            }
        }
    }
}