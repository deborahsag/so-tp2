# Simulação de paginação

## Implementação da tambela de páginas
Uma lista encadeada em que a própria célula guarda os dados. A lista possui uma cabeça dummy que guarda a informação do tamanho da lista. Essa informação foi encapsulada dessa forma para que o tamanho seja aumentado e diminuído nas próprias funções de adicionar e remover páginas.

Uma vez que a paginação não é ordenada na memória, adotou-se a estratégia simplificada de sempre inserir novas páginas no final da lista encadeada.

Tendo em mente a simplicidade de implementação e considerando o tamanho reduzido da tabela de páginas, a busca é realizada sequencialmente pela lista.

## Acesso à memória
O programa é processado enquanto o arquivo vai sendo lido.