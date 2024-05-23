//====== INFORMALÇÕES IMPORTANTES ======//
// O campo m na struct representa o número de nós atual armazenados no nó
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
void menu(struct no** raiz);
//- Função de busca em árvores B
void buscaB(int x, struct no * r, struct no* pt, int* f, int* g);
//- Função que gera um novo no de forma dinamica
struct no* novo_no(int x);
//- Função responsável por fazer a inserção de um novo elemento à árvore
int insercao(struct no** raiz, int x);

int main() {
    struct no* raiz = NULL;

    menu(&raiz);

    //Sistema para imprimir as chaves da raiz para testar os primeiros casos de inserção
    if(raiz == NULL)
        printf("Raiz inválida\n");
    else {
        printf("raiz: ");
        for(int i = 0; i < raiz->m; i++)
            printf("[%d] ", raiz->chaves[i]);
    }
    printf("\n");
    return 0;
}

//- Função que imprime o menu na tela
void menu(struct no** raiz) {
    int opt, x, run = 1;

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

            case 2:
                printf("Insira a nova chave: ");
                scanf("%d", &x);

                if(insercao(raiz, x))
                    printf("Chave inserida!\n");
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
    //Declarando ponteiro para percorrer os nós da árvore, definindo pt para NULL e inicializando *f = 0 => chave não encontrada
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
        while(p != NULL && i < p->m){
            //Caso x seja maior que a chave de índice i
            if(x > p->chaves[i]){
                //atualizamos i para percorrer o array de chaves e *g para armazenar a possível posição onde x será colocado caso não seja encontrado
                i += 1;         
                *g += 1;
            //Caso x seja igual a chave de índice i
            } else if(x == p->chaves[i]){
                //Alteramos p para NULL e *f para 1 => chave encontrada
                p = NULL;
                *f = 1;
            //Caso x seja menor que a chave de índice i
            } else {
                //Vamos para o próximo nó de índice i e atualizamos o valor de i para sair do laço
                p = p->filhos[i];
                if(p != NULL) i = p->m + 1;
            }
        }
        //Caso i não seja menor que nenhuma das chaves já verificadas, enviamos p para o próximo nó
        if(p != NULL && i == p->m)
            p = p->filhos[i];
    }
}

//- Função que gera um novo no de forma dinamica
struct no* novo_no(int x) {
    //Variável que armazena o novo nó criado dinamicamente
    struct no* novo = NULL;

    //Criando novo nó de forma dinâmica e inicializando a primeira chave(campo chaves[0])
    //e seu total de chaves atual(campo m)
    novo = (struct no*) calloc (1, sizeof(struct no));
    novo->chaves[0] = x;
    novo->m = 1;

    //Inicializando todos os ponteiros para os filhos como NULL
    for(int i = 0; i < 2 * D + 1; i++)
        novo->filhos[i] = NULL;
    
    //Retornando novo
    return novo;
}

//- Função responsável por fazer a inserção de um novo elemento à árvore
int insercao(struct no** raiz, int x) {
    //Variáveis de parâmetros para função de busca
    int f, g;
    struct no* pt = NULL;

    //Primeiro verifica se a raiz da árvore é NULL para então criar a raiz da árvore    
    if(*raiz == NULL){
        *raiz = novo_no(x);
    }
    //Caso a raiz da ávore seja diferente de NULL
    else {
        //Chamada da função de busca na árvore B
        buscaB(x, *raiz, pt, &f, &g);
        
        //Verifica se o elemento foi encontrado
        if(f == 1){
            printf("Elemento encontrado!\n");
            return 0;
        }
    }

    //Retorna um ao fim indicando a inserção de um novo elemento a árvore
    return 1;
}