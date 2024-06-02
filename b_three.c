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
//- Função responsável por fazer a inserção de um novo elemento à árvore
int insercao(struct no** raiz, int x);
//- Função imprimir chaves do nó
void imprimirNo(struct no* pt);
//- Função do algoritmo de cisão
void dividirPagina(struct no** r, struct no* pt, int x, int g);
//- Função que gera trata de criar nova página, atualizar página atual e retornar posição
// necessária para cisão
struct no* dividirFolha(int *c, struct no** pt, int x, int g);

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
            //Empurramos os valores a partir de g uma casa pra frente
            for(int i = pt->m; i > g; i--)
                pt->chaves[i] = pt->chaves[i - 1];
            //Adicionamos x na posição g
            pt->chaves[g] = x;
            //Atualizamos o número de chaves atuais no vetor
            pt->m += 1;

        //Caso a folha pt esteja lotada, executar o algoritmo de cisão
        } else {
            dividirPagina(raiz, pt, x, g);
        }
    }

    //Retorna um ao fim indicando a inserção de um novo elemento a árvore
    return 1;
}

//- Função que gera trata de criar nova página, atualizar página atual e retornar posição
// necessária para cisão
struct no* dividirFolha(int *c, struct no** pt, int x, int g) {
    //Cria novo ponteiro que será irmã adjacente da página atual
    struct no* pt_novo = novaPagina(0);

    //Caso g < D...
    if(g < D) {
        //Armazena elemento que se encontra na posição D - 1
        *c = (*pt)->chaves[D - 1];

        //-- Atualização da nova página pt_novo
        //Adiciona as chaves antes da posição do meio ao filho esquerdo
        for(int i = 0; i < g; i++)
            pt_novo->chaves[i] = (*pt)->chaves[i];
        pt_novo->chaves[g] = x;

        //Percorre as casas depois de g
        for(int i = g; i < D - 1; i++)
            pt_novo->chaves[i+1] = (*pt)->chaves[i];
        pt_novo->m = D;

        //-- Atualização da página atual pt
        //Percorrendo vetor atual atualizando as posições dos números trazendo eles para o começo
        //do vetor, mantendo a propriedade de ordenação
        for(int i = D; i < 2 * D; i++)
            (*pt)->chaves[i - D] = (*pt)->chaves[i];
        (*pt)->m = D;
    }
    //Caso g > D...
    else if(g > D) {
        //Armazena elemento que se encontra na posição D
        *c = (*pt)->chaves[D];

        //-- Atualização da nova página pt_novo
        //Adiciona as chaves do pt_novo até a posição D
        for(int i = 0; i < D; i++)
            pt_novo->chaves[i] = (*pt)->chaves[i];
        pt_novo->m = D;

        //-- Atualização da página atual pt
        //Percorrendo vetor atual (partindo de D + 1) atualizando as posições dos até g
        for(int i = D + 1; i < g; i++)
            (*pt)->chaves[i - (D + 1)] = (*pt)->chaves[i];
        (*pt)->chaves[g - (D + 1)] = x;

        //Percorrendo o vetor atual partindo da posição g até 2 * D
        for(int i = g; i < 2 * D; i++)
            (*pt)->chaves[i - D] = (*pt)->chaves[i];
        (*pt)->m = D;
    }
    //Caso g == D...
    else{
        //Armazena x em c, para eviar ele ao pai de pt
        *c = x;

        //-- Atualização da nova página pt_novo
        //Adiciona as chaves da metade de pt em pt_novo
        for(int i = 0; i < D; i++)
            pt_novo->chaves[i] = (*pt)->chaves[i];
        pt_novo->m = D;

        //-- Atualização de pt, trazendo seus últimos valores para frente
        for(int i = D; i < 2*D;i++)
            (*pt)->chaves[i - D] = (*pt)->chaves[i];
        (*pt)->m = D;
    }
    return pt_novo;
}

//- Função do algoritmo de cisão
void dividirPagina(struct no** r, struct no* pt, int x, int g) {
    //Cria um novo ponteiro que será o filho esquerdo
    struct no* pt_novo = NULL, * pt_pai = NULL, * pt_aux = NULL;
    //Calcula a posição do 'meio' de uma folha e armazena o valor encontrado em pt->chaves nessa posição
    int f_aux = 0, g_aux = 0, c = 0;

    //Caso o ponteiro pra fazer a cisão seja a raiz e a mesma seja uma folha
    if(pt == *r && pt->filhos[0] == NULL){
        //Criando nova página adjacente, atualizando pt e retornando chave que será
        //adicionada no pai de pt
        pt_novo = dividirFolha(&c, &pt, x, g);

        *r = novaPagina(c);
        (*r)->filhos[0] = pt_novo;
        (*r)->filhos[1] = pt;
    //Caso contrário...
    } else {
        //Buscando pelo pai de pt...
        pt_pai = buscaB(pt->chaves[0], *r, &pt_aux, &f_aux, &g_aux);

        //Verificando se o pai de pt já está completo...
        if(pt_pai->m == 2*D)
            printf("Caso recursivo");
            //dividirPagina(r, pt_pai, c, g);
        //Caso contrário...
        //Aplicar cisão para páginas internas (nós internos)...
        else{
            //Para descobrir a posição de pt no vetor de filhos de seu pai
            int w = 0;
            while(pt_pai->filhos[w] != pt) w++;
            printf("w: %d\n", w);

            //Caso pt seja uma folha
            if(pt->filhos[0] == NULL) {
                //Dividi a folha
                pt_novo = dividirFolha(&c, &pt, x, g);

                //Movimenta as chaves do pai de pt começando na última indo até a posição de pt em pt_pai->filhos
                for(int i = pt_pai->m; i > w; i--){
                    //printf("%d ", i);
                    pt_pai->chaves[i] = pt_pai->chaves[i - 1];
                }

                //Movimenta os filhos do pai de pt começando pelo último ind até a posição ed pt em pt_pai->filhos
                for(int i = pt_pai->m + 1; i > w + 1; i--)
                    pt_pai->filhos[i] = pt_pai->filhos[i - 1];
                
                pt_pai->chaves[w] = c;

                pt_pai->filhos[w] = pt_novo;
                pt_pai->filhos[w + 1] = pt;
                //Por fim, atualiza o total de filhos de pt_pai
                pt_pai->m += 1;
            }
        }
    }
}