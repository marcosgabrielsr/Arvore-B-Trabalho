//====== Incluindo bibliotecas
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//====== Definindo constantes
#define D 2
#define TAMANHO_MAXIMO_NOME 13

//====== Criando structs
struct no{
    int m;                          //Campo que armazena o número de chaves atual do nó
    int chaves[2 * D];
    struct no * filhos[2 * D + 1];
};

//====== Declarando protótipos das funções
//- Função que imprime o menu na tela
void menu(struct no** raiz);
//- Função de busca em árvores B
struct no* buscaB(int x, struct no* r, struct no** pt, int* f, int* g);
//- Função que gera um novo no de forma dinamica
struct no* novaPagina(int x);
//- Função de comparar para a função qsort
int comparar(const void * a, const void * b);
//- Função responsável por fazer a inserção de um novo elemento à árvore
int insercao(struct no** raiz, int x);
//- Função imprimir chaves do nó
void imprimirNo(struct no* pt);
//- Função que gera trata de criar nova nova folha, atualizar a folha atual e retornar posição
// necessária para cisão
void dividirFolha(struct no** r, struct no** pt, int x, int g);
//- Função responsável por fazer a divisão de nós(páginas) internos
void dividirNoInt(struct no** r, struct no** pt, int x, int w, struct no* pt1, struct no* pt2);
//- Função que retorna o pai de pt e a posição do filho no vetor de filhos do pai
struct no* pai(struct no* r, struct no* pt, int *i);

int main() {
    struct no* raiz = NULL;

    menu(&raiz);

    printf("\n");
    return 0;
}

//- Função que imprime o menu na tela
void menu(struct no** raiz) {
    int opt, x, f, g, run = 1;
    struct no* pt, * pt_pai;

    do {
        printf("\n// ----- // ----- // ÁRVORE B // ----- // ----- //\n");
        printf("[1] - Buscar\n");
        printf("[2] - Inserir\n");
        printf("[3] - Remover\n");
        printf("[4] - Imprimir Raiz\n");
        printf("[9] - Finalizar\n");

        printf("Opção escolhida: ");
        scanf("%d", &opt);

        switch(opt) {
            case 1:
                printf("Insira a chave a ser buscada: ");
                scanf("%d", &x);
                pt_pai = buscaB(x, *raiz, &pt, &f, &g);

                if(f == 1)
                    printf("Elemento encontrado no endereço: %p\nPai do Elemento: %p\n", pt, pt_pai);
                else
                    printf("Elemento não encontrado\n");
            break;

            case 2:
                printf("Insira a nova chave: ");
                scanf("%d", &x);

                if(insercao(raiz, x))
                    printf("Chave inserida!\n");
                else
                    printf("Inserção inválida!\n");
            break;

            case 3:
                printf("Remover\n");
            break;

            case 4:
                //Sistema para imprimir as chaves da raiz para testar os primeiros casos de inserção
                if(*raiz == NULL)
                    printf("Raiz nula\n");
                else {
                    printf("raiz(%p): ", *raiz);
                    imprimirNo(*raiz);

                    for(int i = 0; i <= (*raiz)->m; i++){
                        printf("pt(%p)->filhos[%d]:", (*raiz)->filhos[i], i);
                        imprimirNo((*raiz)->filhos[i]);
                        printf("Imprimir filhos de filho[%d]\n", i);
                        for(int j = 0; j <= ((*raiz)->filhos[i])->m; j++)
                            imprimirNo(((*raiz)->filhos[i])->filhos[j]);
                    } 
                }
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

//- Função de comparar para a função qsort
int comparar(const void * a, const void * b) {
    return ( *(int*)a - *(int*)b );
}

//- Função imprimir chaves do nó
void imprimirNo(struct no* pt) {
    if(pt == NULL)
        printf("NULL");
    else
        for(int i = 0; i < pt->m; i++)
            printf(" [%d]", pt->chaves[i]);

    printf("\n");
}

//- Função de busca em árvores B
struct no* buscaB(int x, struct no* r, struct no** pt, int* f, int* g) {
    //Declarando ponteiro para percorrer os nós da árvore, definindo pt para NULL e inicializando *f = 0 => chave não encontrada
    struct no* p = r;
    struct no* pai;
    *pt = NULL;
    *f = 0;

    //Enquanto o nó não for encontrado e enquanto ele não for uma folha
    while(p != NULL){
        //Inicializando i para marcar os índices, g para armazenar a posição em que o possível novo nó deveria ser incluso
        //e armazenando o ponteiro da busca em pt
        int i = 0;
        *g = 0;
        pai = *pt;
        *pt = p;
        
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

    return pai;
}

//- Função que gera um novo no de forma dinamica
struct no* novaPagina(int x) {
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
    if(*raiz == NULL)
        *raiz = novaPagina(x);
    //Caso a raiz da ávore seja diferente de NULL
    else {
        //Chamada da função de busca na árvore B
        buscaB(x, *raiz, &pt, &f, &g);
        //Verifica se o elemento foi encontrado
        if(f == 1)
            return 0;
        //Caso o elemento não foi encontrado, ou seja pt->m < 2 * D
        if(pt->m < 2*D) {
            //Inclui x na posição m
            pt->chaves[pt->m] = x;
            pt->m += 1;
            //Ordena pt->chaves
            qsort(pt->chaves, pt->m, sizeof(int), comparar);

        //Caso a folha pt esteja lotada, executar o algoritmo de cisão
        } else {
            dividirFolha(raiz, &pt, x, g);
        }
    }
    //Retorna um ao fim indicando a inserção de um novo elemento a árvore
    return 1;
}

//- Função que retorna o pai de pt e a posição do filho no vetor de filhos do pai
struct no* pai(struct no* r, struct no* pt, int *i) {
    //Variáveis auxiliares para a busca
    struct no* pt_aux,* pt_pai;
    int f_aux, g_aux;

    //Armazenando pai de pt
    pt_pai = buscaB(pt->chaves[0], r, &pt_aux, &f_aux, &g_aux);

    //Encontrando a posição de pt no vetor de filhos de pt_pai
    *i = 0;
    while(pt_pai->filhos[*i] != pt) *i += 1;

    return pt_pai;
}

//- Função que gera trata de criar nova página, atualizar página atual e retornar posição
// necessária para cisão
void dividirFolha(struct no** r, struct no** pt, int x, int g) {
    //Vetor para armazenar as chaves da folha mais x
    int vetor[2*D+1];
    //Cria novo ponteiro que será irmã adjacente da página atual
    struct no* pt_novo = novaPagina(0);
    
    //Copia o vetor pt->chaves para vetor e adiciona x
    memcpy(vetor, (*pt)->chaves, (2*D)*sizeof(int));
    vetor[2*D] = x;
    //Ordenando o vetor
    qsort(vetor, 2*D+1, sizeof(int), comparar);

    //Copiando os elementos antes da posição do meio para pt_novo
    memcpy(pt_novo->chaves, vetor, D*sizeof(int));
    //Copiando os elementos depois da ṕosição do meio para pt
    memcpy((*pt)->chaves, &vetor[(2*D+1)/2 + 1], D*sizeof(int));
    //Atualizando número de chaves de pt e pt_novo
    (*pt)->m = pt_novo->m = D;

    if(*pt == *r){
        //Cria uma nova raiz com o elemento do meio do vetor
        *r = novaPagina(vetor[(2*D + 1)/2]);
        (*r)->filhos[0] = pt_novo;
        (*r)->filhos[1] = *pt;
    } else {
        //Armazenando o pai de pt (pt_pai) a posição de w em pt_pai->filho
        int w;
        struct no* pt_pai = pai(*r, *pt, &w);
        //Caso pai de pt esteja cheio...
        if(pt_pai->m == 2*D) {
            dividirNoInt(r, &pt_pai, vetor[(2*D + 1)/2], w, pt_novo, *pt);
        //Caso contrário...
        } else {
            //Movimenta as chaves do pai de pt começando na última indo até a posição de pt em pt_pai->filhos
            memmove(&pt_pai->chaves[w + 1], &pt_pai->chaves[w], (pt_pai->m - w) * sizeof(int));
            //Movimenta as chaves do pai de pt começando na última indo até a posição de pt em pt_pai->filhos
            memmove(&pt_pai->filhos[w + 2], &pt_pai->filhos[w + 1], (pt_pai->m - w + 1) * sizeof(int));
            
            pt_pai->chaves[w] = vetor[(2*D + 1)/2];
            pt_pai->filhos[w] = pt_novo;
            pt_pai->filhos[w + 1] = *pt;
            //Por fim, atualiza o total de filhos de pt_pai
            pt_pai->m += 1;
        }
    }
}

//- Função responsável por fazer a divisão de nós(páginas) internos
void dividirNoInt(struct no** r, struct no** pt, int x, int w, struct no* pt1, struct no* pt2) {
    //Nova página
    struct no* pt_novo = novaPagina(0);
    int vM;

    if(w > D) {
        //Armazena o valor médio
        vM = (*pt)->chaves[D];

        //Armazena as chaves do intervalo [0:D] para pt_novo
        for(int i = 0; i < D; i++)
            pt_novo->chaves[i] = (*pt)->chaves[i];
        //Armazena os filhos do intervalo [0:w] para pt_novo
        for(int i = 0; i < w; i++)
            pt_novo->filhos[i] = (*pt)->filhos[i];
        pt_novo->m = D;

         if(w > D + 1){        
            //Atualizando a posição das chaves de pt
            for(int i = D + 1; i < w; i++)
                (*pt)->chaves[i - (D + 1)] = (*pt)->chaves[i];
            (*pt)->chaves[w - (D + 1)] = x;

            //Atualizando as posições das chaves de pt depois de w
            for(int i = w + 1; i < 2*D; i++)
                (*pt)->chaves[i - D] = (*pt)->chaves[i];
            (*pt)->m = D;

            //Atualizando a posição dos filhos de pt
            for(int i = D + 1; i < w; i++)
                (*pt)->filhos[i - (D + 1)] = (*pt)->filhos[i];
        //Caso contrário...
        } else {
            //Atualizando a posição das chaves de pt
            (*pt)->chaves[0] = x;
            for(int i = D + 1; i < 2*D;i++)
                (*pt)->chaves[i - D] = (*pt)->chaves[i];
            (*pt)->m = D;

            //Atualizando a posição dos filhos de pt
            for(int i = D + 1; i < 2*D + 1; i++)
                (*pt)->filhos[i - D] = (*pt)->filhos[i];
        }

        //Adicionando os ponteiros obtidos da divisão do filho de pt aos vetor de filhos de pt
        (*pt)->filhos[w - (D + 1)] = pt1;
        (*pt)->filhos[(w + 1) - (D + 1)] = pt2;
    
    } else if(w < D) {
        //Armazena o valor médio
        vM = (*pt)->chaves[D - 1];

        if(w == 0) {
            //Armazena x na primeira posição de pt_novo->chaves
            pt_novo->chaves[0] = x;

            //Copiando as chaves do intervalo [0:D-1] de pt para [1:D] de pt_novo
            for(int i = 0; i < D - 1; i++)
                pt_novo->chaves[i + 1] = (*pt)->chaves[i];
            pt_novo->m = D;

            //Copiando os filhos de pt do intervalo [0:D] de pt para [1:D + 1] de pt_novo
            for(int i = 0; i < D; i++)
                pt_novo->filhos[i + 1] = (*pt)->filhos[i];
            
            //Atualiza os filhos de pt_novo adicionando pt1 e pt2
            pt_novo->filhos[0] = pt1;
            pt_novo->filhos[1] = pt2;
        } else {
            //Copiando as chaves do intervalo [0:w] de pt para pt_novo
            for(int i = 0; i < w; i++)
                pt_novo->chaves[i] = (*pt)->chaves[i];
            //Adicionando x na posição w de de pt_novo->chave
            pt_novo->chaves[w] = x;
            //Copiando as chaves do intervalo [w+1:D-1] de pt para pt_novo
            for(int i = w + 1; i < D - 1; i++)
                pt_novo->chaves[i] = (*pt)->chaves[i];
        }

        //Atualizando as chaves pt a partir de D para o começo de pt
        for(int i = D; i < 2*D; i++)
            (*pt)->chaves[i - D] = (*pt)->chaves[i];

        //Atualizando os filhos de pt para o começo de pt
        for(int i = D; i < 2*D + 1; i++)
            (*pt)->filhos[i - D] = (*pt)->filhos[i];
        //Atualizando total de chaves de pt
        (*pt)->m = D;
    }
    if(*pt == *r){
        *r = novaPagina(vM);
        (*r)->filhos[0] = pt_novo;
        (*r)->filhos[1] = *pt;
    }
}