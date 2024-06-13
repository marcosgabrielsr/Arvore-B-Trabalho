//====== Incluindo bibliotecas
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//====== Definindo constantes
#define D 4
#define TAMANHO_MAXIMO_NOME 13

//====== Criando structs
struct no{
    int m;                                      //Campo que armazena o número de chaves atual do nó
    char chaves[2 * D][TAMANHO_MAXIMO_NOME];
    struct no * filhos[2 * D + 1];
};

//====== Declarando protótipos das funções
//- Função que imprime o menu na tela
void menu(struct no** raiz);
//- Função de busca em árvores B
struct no* buscaB(char x[TAMANHO_MAXIMO_NOME], struct no* r, struct no** pt, int* f, int* g);
//- Função que gera um novo no de forma dinamica
struct no* novaPagina(char x[TAMANHO_MAXIMO_NOME]);
//- Função de comparação para vetores inteiros na função qsort
int compararChaves(const void * a, const void * b);
//- Função de comparação para ponteiros para no na função qsort
int compararNo(const void * a, const void * b);
//- Função responsável por fazer a inserção de um novo elemento à árvore
int insercao(struct no** raiz, char x[TAMANHO_MAXIMO_NOME]);
//- Função imprimir chaves do nó
void imprimirNo(struct no* pt);
//- Função que divide as chaves para dois ponteiros na hora da cisao e retorna o valor medio em x
void distribuirChaves(const int n, struct no** pt, struct no** qt, char x[TAMANHO_MAXIMO_NOME]);
//- Função que divide os filhos para dois ponteiros na hora da cisao
void distribuirFilhos(const int n, struct no** pt, struct no** qt, struct no* novo_filho);
//- Função que gera trata de criar nova nova folha, atualizar a folha atual e retornar posição necessária para cisão
void dividirPagina(struct no** r, struct no** pt, char x[TAMANHO_MAXIMO_NOME], struct no* novo_filho);
//- Função que retorna o pai de pt e a posição do filho no vetor de filhos do pai
struct no* pai(struct no* r, struct no* pt, int *i);
//- Organiza o pai do nó que não está cheio quando o nó sofre cisão
void organizaPai(struct no** pt_pai, struct no* pt_novo, char x[TAMANHO_MAXIMO_NOME]);
//- Função que remove um elemento da árvore
int remover(struct no** r, char x[TAMANHO_MAXIMO_NOME]);
//- Pega a maior chave da filha seguindo os filhos de pt
void pegarMaior(struct no** pt, int i, struct no** folha);
//- Função que anula a raiz caso sua quantidade de chaves seja 0 (r->m == 0)
void anularRaiz(struct no** r);
//- Função que concatena páginas
void concatenar(struct no **r, struct no** pt_pai, struct no** pt, struct no** qt, int w);
//- Função que redistribui chaves de duas páginas
void redistribuir(struct no** pt_pai, struct no** pt, struct no** qt, int w);
//- Função que carrega os nomes dos pokémons para a árvore B
int carregarDados(char nameFile[], struct no** r);

int main() {
    //Criando raiz e ponteiro para arquivo
    struct no* raiz = NULL;
    
    if(carregarDados("pokemon_names.txt", &raiz))
        menu(&raiz);
    else
        printf("Erro ao encontrar o arquivo\n");

    return 0;
}

//- Função que imprime o menu na tela
void menu(struct no** raiz) {
    int opt, f, g, executar = 1;
    char x[TAMANHO_MAXIMO_NOME];
    struct no* pt;

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
                scanf(" %[^\n]", x);
                buscaB(x, *raiz, &pt, &f, &g);

                if(f == 1)
                    printf("Endereço: %p\nElemento: %s", pt, pt->chaves[g]);
                else
                    printf("Elemento não encontrado\n");
            break;

            case 2:
                printf("Insira a nova chave: ");
                scanf(" %[^\n]", x);

                if(insercao(raiz, x))
                    printf("Chave inserida!\n");
                else
                    printf("Inserção inválida!\n");
            break;

            case 3:
                printf("Insira a chave a ser removida: ");
                scanf(" %[^\n]", x);

                if(remover(raiz, x))
                    printf("Chave Removida!\n");
                else
                    printf("Remoção inválida!\n");
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
                        if((*raiz)->filhos[i] != NULL)
                            for(int j = 0; j <= ((*raiz)->filhos[i])->m; j++)
                                imprimirNo(((*raiz)->filhos[i])->filhos[j]);
                    } 
                }
            break;
        
            case 9:
                executar = 0;
                printf("Saindo...\n");
            break;

            default:
                printf("Opção inválida. Tente novamente...\n");
            break;
        }
    } while(executar == 1);
}

//- Função que carrega os nomes dos pokémons para a árvore B
int carregarDados(char fileName[], struct no** r) {
    FILE *file;

    //Abrindo arquivo de nome
    file = fopen(fileName, "r");
    //Caso o arquivo não foi encontrado...
    if(file == NULL)
        return 0;
    //string para armazenar nome dos pokemons lidos do arquivo em cada linha
    char pokemon[TAMANHO_MAXIMO_NOME];
    //percorrendo arquivo até chegar ao fim...
    while(fgets(pokemon, sizeof(pokemon), file)){
        //Removendo caractere \n
        char *p = strchr(pokemon, '\n');
        if(p != NULL) *p = '\0';
        insercao(r, pokemon);
    }
    //Fechando arquivo
    fclose(file);
    return 1;
}

//- Função de comparar para a função qsort
int compararChaves(const void * a, const void * b) {
    return strcmp((const char *)a, (const char *)b);
}

//- Função de comparação para ponteiros para no na função qsort
int compararNo(const void * a, const void * b) {
    struct no *noA = *(struct no **)a;
    struct no *noB = *(struct no **)b;

    //Tratamentos de casos para caso vetores sejam nulos
    if (noA == NULL && noB == NULL) {
        return 0;
    } else if (noA == NULL) {
        return 1;  // NULLs vão para o fim
    } else if (noB == NULL) {
        return -1; // NULLs vão para o fim
    } else {
        return strcmp(noA->chaves[0], noB->chaves[0]);
    }
}

//- Função imprimir chaves do nó
void imprimirNo(struct no* pt) {
    if(pt == NULL)
        printf("NULL");
    else
        for(int i = 0; i < pt->m; i++)
            printf(" [%s]", pt->chaves[i]);

    printf("\n");
}

//- Função de busca em árvores B
struct no* buscaB(char x[TAMANHO_MAXIMO_NOME], struct no* r, struct no** pt, int* f, int* g) {
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
            if(strcmp(x, p->chaves[i]) > 0){
                //atualizamos i para percorrer o array de chaves e *g para armazenar a possível posição onde x será colocado caso não seja encontrado
                i += 1;         
                *g += 1;

            //Caso x seja igual a chave de índice i
            } else if(strcmp(x, p->chaves[i]) == 0){
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
struct no* novaPagina(char x[TAMANHO_MAXIMO_NOME]) {
    //Variável que armazena o novo nó criado dinamicamente
    struct no* novo = NULL;

    //Criando novo nó de forma dinâmica e inicializando a primeira chave(campo chaves[0])
    //e seu total de chaves atual(campo m)
    novo = (struct no*) calloc (1, sizeof(struct no));
    strcpy(novo->chaves[0], x);
    novo->m = 1;

    //Inicializando todos os ponteiros para os filhos como NULL
    for(int i = 0; i < 2 * D + 1; i++)
        novo->filhos[i] = NULL;
    
    //Retornando novo
    return novo;
}

//- Função responsável por fazer a inserção de um novo elemento à árvore
int insercao(struct no** raiz, char x[TAMANHO_MAXIMO_NOME]) {
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
            strcpy(pt->chaves[pt->m], x);
            pt->m += 1;
            //Ordena pt->chaves
            qsort(pt->chaves, pt->m, TAMANHO_MAXIMO_NOME, compararChaves);
        //Caso a folha pt esteja lotada, executar o algoritmo de cisão
        } else {
            dividirPagina(raiz, &pt, x, NULL);
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

//- Organiza o pai do nó que não está cheio quando o nó sofre cisão
void organizaPai(struct no** pt_pai, struct no* pt_novo, char x[TAMANHO_MAXIMO_NOME]) {
    //Sobe o valor medio para pt_pai e ordena o vetor de chaves
    strcpy((*pt_pai)->chaves[(*pt_pai)->m], x);
    (*pt_pai)->m += 1;
    qsort((*pt_pai)->chaves, (*pt_pai)->m, TAMANHO_MAXIMO_NOME, compararChaves);

    //Sobe pt_novo para pt_pai e ordena o vetor de filhos
    (*pt_pai)->filhos[(*pt_pai)->m] = pt_novo;
    qsort((*pt_pai)->filhos, (*pt_pai)->m + 1, sizeof(struct no*), compararNo);
}

//- Função que divide as chaves para dois ponteiros na hora da cisao e retorna o valor medio em x
void distribuirChaves(const int n, struct no** pt, struct no** qt, char x[TAMANHO_MAXIMO_NOME]) {
    //Vetor para armazenar as chaves dos ponteiros mais x
    char vetor[n][TAMANHO_MAXIMO_NOME];
    //Copia vetor pt->chaves para vetor e adiciona x
    memcpy(vetor, (*pt)->chaves, (n - 1)*TAMANHO_MAXIMO_NOME*sizeof(char));
    strcpy(vetor[n - 1], x);
    //Ordenando o vetor
    qsort(vetor, n, TAMANHO_MAXIMO_NOME, compararChaves);
    //Copiando os elementos antes da posição do meio para pt_novo
    memcpy((*qt)->chaves, vetor, D*TAMANHO_MAXIMO_NOME*sizeof(char));
    //Copiando os elementos depois da posição do meio para pt
    memcpy((*pt)->chaves, &vetor[(2*D+1)/2 + 1], D*TAMANHO_MAXIMO_NOME*sizeof(char));
    //Copia o valor medio para x para ser usado depois
    strcpy(x,vetor[n/2]);
}

//- Função que divide os filhos para dois ponteiros na hora da cisao
void distribuirFilhos(const int n, struct no** pt, struct no** qt, struct no* novo_filho) {
    //Vetor para armazenar os filhos dos ponteiros mais novo_filho
    struct no* vetorF[n];
    //Copia o vetor pt->filhos para o vetorF e adiciona novo_filho
    memcpy(vetorF, (*pt)->filhos, (2*D +1)*sizeof(struct no*));
    vetorF[n - 1] = novo_filho;
    //Ordenando vetorF
    qsort(vetorF, n, sizeof(struct no*), compararNo);

    //Copiando os primeiros D+1 valores para pt_novo->filhos
    memcpy((*qt)->filhos, vetorF, (n/2)*sizeof(struct no*));
    //Copiando os D+1 a partir do meio de vetorF para (*pt)->filhos
    memcpy((*pt)->filhos, &vetorF[n/2], (n/2)*sizeof(struct no*));
}

//- Função que gera trata de criar nova página, atualizar página atual e retornar posição
// necessária para cisão
void dividirPagina(struct no** r, struct no** pt, char x[TAMANHO_MAXIMO_NOME], struct no* novo_filho) {
    //Cria novo ponteiro que será irmã adjacente da página atual
    struct no* pt_novo = novaPagina(x);
    //Distribuindo as chaves para ambos
    distribuirChaves(2*D + 1, pt, &pt_novo, x);
    //Atualizando número de chaves de pt e pt_novo
    (*pt)->m = pt_novo->m = D;

    //Caso pt não seja uma folha
    if((*pt)->filhos[0] != NULL)
        distribuirFilhos(2*D + 2, pt, &pt_novo, novo_filho);
    
    if(*pt == *r){
        //Cria uma nova raiz com o elemento do meio do vetor
        *r = novaPagina(x);
        (*r)->filhos[0] = pt_novo;
        (*r)->filhos[1] = *pt;
    } else {
        //Armazenando o pai de pt (pt_pai) a posição de w em pt_pai->filho
        int w;
        struct no* pt_pai = pai(*r, *pt, &w);
        //Caso pai de pt esteja cheio...
        if(pt_pai->m == 2*D) {
            dividirPagina(r, &pt_pai, x, pt_novo);
        //Caso contrário...
        } else {
            organizaPai(&pt_pai, pt_novo, x);
        }
    }
}

//- Pega a maior chave da filha seguindo os filhos de pt
void pegarMaior(struct no** pt, int i, struct no** folha) {
    //Ponteiro auxiliar para percorrer os descendentes de pt
    struct no* p = (*pt)->filhos[i];
    
    //Percorrendo até chegar a folha
    while(p->filhos[0] != NULL)
        p = p->filhos[p->m];
    //Armazena a maior chave da folha
    strcpy((*pt)->chaves[i], p->chaves[p->m - 1]);
    //Armazena a folha e atualiza o seu tamanho
    *folha = p;
    p->m -= 1;
}

//-Função que anula a raiz caso sua quantidade de chaves seja 0 (r->m == 0)
void anularRaiz(struct no** r){
    struct no* p = *r;
    *r = NULL;
    free(p);
}

//- Função que remove um elemento da árvore
int remover(struct no** r, char x[TAMANHO_MAXIMO_NOME]) {
    //Variáveis para a busca
    struct no* pt = NULL, * folha = NULL;
    int f, g;
    //Executando busca
    buscaB(x, *r, &pt, &f, &g);
    //Caso o elemento não foi encontrado...
    if(f == 0)
        return 0;
    //Caso contrário...
    else{
        //Verifica se é uma folha...
        if(pt->filhos[0] == NULL) {
            folha = pt;
            //Sobrescreve o vetor com memmove para remover a antiga chave
            memcpy(&(pt->chaves[g]), &(pt->chaves[g + 1]), (pt->m-(g + 1)) * TAMANHO_MAXIMO_NOME * sizeof(int));
            pt->m -= 1;
            //Caso pt seja a raiz e a mesma não tenha chaves
            if(pt == *r && pt->m == 0)
                anularRaiz(r);
        //Caso contrário...
        } else {
            //Pega o maior elemento da folha e coloca na posição de x
            pegarMaior(&pt, g, &folha);
        }

        //Verifica se o número atual de chaves de folha é menor que D
        if(folha != *r && folha->m < D) {
            //Pega o pai de folha e sua posição em relação a seu pai
            int w;
            struct no* pt_pai = pai(*r, folha, &w);
            //Caso w seja pt_pai->m - 1...
            if(w == 0) {
                if((pt_pai->filhos[w])->m + (pt_pai->filhos[w + 1])->m < 2*D)
                    concatenar(r, &pt_pai, &pt_pai->filhos[w], &pt_pai->filhos[w + 1], w);
                else
                    redistribuir(&pt_pai, &pt_pai->filhos[w], &pt_pai->filhos[w + 1], w);
            } else {
                if((pt_pai->filhos[w - 1])->m + (pt_pai->filhos[w])->m < 2*D)
                    concatenar(r, &pt_pai, &pt_pai->filhos[w - 1], &pt_pai->filhos[w], w - 1);
                else
                    redistribuir(&pt_pai, &pt_pai->filhos[w - 1], &pt_pai->filhos[w], w - 1);
            }
        }
    }
    return 1;
}

//- Função que concatena páginas
void concatenar(struct no **r, struct no** pt_pai, struct no** pt, struct no** qt, int w) {
    //Vetor de auxílio para concatenar as páginas e adicionar a chave que os separa
    const int n = (*pt)->m + (*qt)->m + 1;
    char vetorC[n][TAMANHO_MAXIMO_NOME];
    //Vetor de auxílio para concatenar os filhos das páginas
    struct no* vetorF[2*D+1];
    struct no* aux = (*qt);
    //Adiciona as chaves de pt, qt e a chave que os divide em vetorC
    memcpy(vetorC, (*pt)->chaves, (*pt)->m*TAMANHO_MAXIMO_NOME*sizeof(char));
    memcpy(&vetorC[(*pt)->m], (*qt)->chaves, (*qt)->m*TAMANHO_MAXIMO_NOME*sizeof(char));
    strcpy(vetorC[(*pt)->m + (*qt)->m], (*pt_pai)->chaves[w]);
    //Ordenando vetorC
    qsort(vetorC, n, TAMANHO_MAXIMO_NOME, compararChaves);

    //Caso pt não seja uma folha
    if((*pt)->filhos[0] != NULL) {
        //Copia os filhos de pt e qt para vetorF
        memcpy(vetorF, (*pt)->filhos, ((*pt)->m + 1)*sizeof(struct no*));
        memcpy(&vetorF[(*pt)->m + 1], (*qt)->filhos, ((*qt)->m + 1)*sizeof(struct no*));
        //Ordenando vetorF
        qsort(vetorF, n + 1, sizeof(struct no*), compararNo);
        //Copiando os dados de vetorF para o vetor de filhos de pt
        memcpy((*pt)->filhos, vetorF, (n+1)*sizeof(struct no*));
    }

    //Copia os dados de vetorC para pt
    memcpy((*pt)->chaves, vetorC, n * TAMANHO_MAXIMO_NOME * sizeof(char));
    (*pt)->m = n;
    //Libera qt
    (*qt) = NULL;
    free(aux);
    
    //Sobreescrevendo as chaves e os filhos de pt_pai uma casa a menos
    memmove(&(*pt_pai)->chaves[w], &(*pt_pai)->chaves[w + 1], ((*pt_pai)->m - (w + 1))*TAMANHO_MAXIMO_NOME*sizeof(char));
    memmove(&(*pt_pai)->filhos[w + 1], &(*pt_pai)->filhos[w + 2], ((*pt_pai)->m + 1 - (w + 2))*sizeof(struct no*));
    //Por fim decrementa o número de chaves de pt
    (*pt_pai)->m -= 1;

    if((*pt_pai) == (*r)){
        //Caso o número de chaves da raiz seja 0...
        if((*pt_pai)->m == 0) {
            //Libera a raiz e a coloca em outro ponteiro
            aux = *r;
            *r = *pt;
            free(aux);
        }
    } else {
        //Verifica se pt_pai possui m < D para determinar ajustes que irão manter as propriedades...
        if((*pt_pai)->m < D){
            //Armazenando avo de pt (pai de pt_pai)
            int j;
            struct no* pt_avo = pai(*r, *pt_pai, &j);
            //Caso j seja pt_pai->filhos[0]...
            if(j == 0) {
                if((pt_avo->filhos[j])->m + (pt_avo->filhos[j + 1])->m < 2*D)
                    concatenar(r, &pt_avo, &pt_avo->filhos[j], &pt_avo->filhos[j + 1], j);
                else
                    redistribuir(&pt_avo, &pt_avo->filhos[j], &pt_avo->filhos[j + 1], j);
            } else {
                if((pt_avo->filhos[j - 1])->m + (pt_avo->filhos[j])->m < 2*D)
                    concatenar(r, &pt_avo, &pt_avo->filhos[j - 1], &pt_avo->filhos[j], j - 1);
                else
                    redistribuir(&pt_avo, &pt_avo->filhos[j - 1], &pt_avo->filhos[j], j - 1);
            }
        }
    }
}   

//- Função que redistribui chaves de duas páginas
void redistribuir(struct no** pt_pai, struct no** pt, struct no** qt, int w) {
    //Variável que armazena o número de elementos do vetor auxiliar de chaves
    const int n = (*pt)->m + (*qt)->m + 1;
    //vetor auxiliar que conterá todos os elementos de pt, qt e mais a chave que os divide
    char vetorC[n][TAMANHO_MAXIMO_NOME];
    struct no** vetorF[n + 1];

    //Copiando as chaves de pt e qt para vetorC e adiciona a chave que os divide
    memcpy(vetorC, (*pt)->chaves, (*pt)->m*TAMANHO_MAXIMO_NOME*sizeof(char));
    memcpy(&vetorC[(*pt)->m], (*qt)->chaves, (*qt)->m*TAMANHO_MAXIMO_NOME*sizeof(char));
    strcpy(vetorC[n - 1], (*pt_pai)->chaves[w]);
    //Ordenando vetorC
    qsort(vetorC, n, TAMANHO_MAXIMO_NOME, compararChaves);
    //Caso pt não seja uma folha
    if((*pt)->filhos[0] != NULL){
        memcpy(vetorF, (*pt)->filhos, ((*pt)->m + 1)*sizeof(struct no*));
        memcpy(&vetorF[(*pt)->m + 1], (*qt)->filhos, ((*qt)->m + 1)*sizeof(struct no*));
        //Ordenando vetorF
        qsort(vetorF, n + 1, sizeof(struct no*), compararNo);
    }
    
    //Copia os elementos antes da metade do vetorC para pt e depois da metade para qt
    memcpy((*pt)->chaves, vetorC, (n/2)*TAMANHO_MAXIMO_NOME*sizeof(char));
    memcpy((*qt)->chaves, &vetorC[n/2 + 1], (n - (n/2 + 1))*TAMANHO_MAXIMO_NOME*sizeof(char));
    (*pt)->m = n/2;
    (*qt)->m = (n - n/2 - 1);

    //Caso pt  não seja uma folha
    if((*pt)->filhos[0] != NULL){
        //Copiando os primeiros n/2 + 1 filhos para pt e o restante para qt
        memcpy((*pt)->filhos, vetorF, (n/2+1)*sizeof(struct no*));
        memcpy((*qt)->filhos, &vetorF[n/2+1], ((n + 1) - (n/2 + 1))*sizeof(struct no*));
    }
    //Atualizando a chave de pt_pai que separa pt e qt
    strcpy((*pt_pai)->chaves[w], vetorC[n/2]);
}