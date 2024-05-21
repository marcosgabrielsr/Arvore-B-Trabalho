//====== INFORMALÇÕES IMPORTANTES ======//
//
//

//====== Incluindo bibliotecas
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//====== Definindo constantes
#define D 2
#define TAMANHO_MAXIMO_NOME 13

//====== Criando structs
struct no{
    int m;
    int chaves[2 * D];
    struct no * filhos[2 * D + 1];
};

//====== Declarando protótipos das funções
//- Função que imprime o menu na tela
void menu();
//- Função de busca em árvores B
void buscaB(int x, struct no * r, struct no* pt, int* f, int* g);
//- Função responsável por fazer a inserção de um novo elemento à árvore
void insercao();

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
    struct no* p = r;
    pt = NULL;
    *f = 0;

    //Enquanto o nó não for encontrado e enquanto ele não for uma folha
    while(p != NULL){
        //Inicializando i para marcar os índices, g para armazenar a posição em que o possível novo nó deveria ser incluso
        //e armazenando o ponteiro da busca em pt
        int i = 0;
        *g = 0;
        pt = p;
        
        //Enquanto i for menos que o número de chaves atual do nó
        while(i < p->m){
            //Caso x seja maior que a chave de índice i
            if(x > p->chaves[i]){
                i += 1;         
                *g += 1;
            //Caso x seja igual a chave de índice i
            } else if(x == p->chaves){
                //Alteramos p para NULL e *f para 1 => chave encontrada
                p = NULL;
                *f = 1;
            //Caso x seja menor que a chave de índice i
            } else {
                //Vamos para o próximo nó de índice i e atualizamos o valor de i para sair do laço
                p = p->filhos[i];
                i = p->m + 1;
            }
            
            //Caso i não seja menor que nenhuma das chaves já verificadas, enviamos p para o próximo nó
            if(i == p->m)
                p = p->filhos[i];
        }
    }
}