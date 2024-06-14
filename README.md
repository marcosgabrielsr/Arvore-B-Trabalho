# Trabalho-B-Arvore
## Descrição:
Trabalho da Faculdade onde Devemos criar uma B-Árvore onde as chaves são nomes de pokémons. Todos os Algoritmo encontrados neste trabalho foram baseados nos pseudo-códigos e explicações do livro "Estruturas de Dados e Seus Algoritmos - Jayme Luiz Szwarcfiter | Lilian Markenzon - 3a Edição".

Note que, a base veio do livro mas os algoritmos são puramente baseados na minha interpretação do livro, por conta disso, creio que há maneiras melhores de escrever este código que chegariam no mesmo resultado.

## Funções
## Busca:
A função de busca foi baseada no algoritmo do livro, no entanto foram feitas algumas alterações. Neste código, a função de busca retorna o pai do ponteiro que contém a chave que procuramos e dentro da estrutura do código em si alguns valores foram alterados, pois no pseudocódigo os índices dos vetores começam com 1 e não 0.

## Inserção e Cisão:
O algoritmo de inserção verifica se a folha está cheia antes de inserir uma nova chave nela, caso ela esteja cheia é chamado o algoritmo de cisão (dividirPagina), caso contrário a inserção é feita normalmente.

O algoritmo de cisão funciona tanto para folhas quanto para nós internos. Ele foi baseado na explicação do livro, de forma que ele divide o vetor de chaves e filhos de uma página em dois e envia para o pai da página a chave que ocupa a posição do meio. Caso a página em questão seja a raiz, é criada uma nova raiz que terá como chave o valor do meio e dois filhos.

## Remoção:
O algoritmo de remoção segue a explicação do livro, de forma que sempre alteramos os valores das folhas e depois chamamos os algoritmos de Concatenação e Redistribuição que podem ser propagados ao longo das páginas.

## Concatenação e Redistribuição:
Os algoritmo de Concatenação e de Redistribuição aplicam a explicação do livro. Para cada chamada destes algoritmos para uma determinada página, o irmão adjacente considerado é o índice esquerdo para quando o índice da página em seu pai é diferente de zero, para quando este índice é zero o irmão adjacente considerado é o de índice 1.