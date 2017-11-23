# GraphOtim

Alocação de frequências em redes de fibra ótica.

## Definição do Problema

Em um grafo não-direcionado G, os nós representam roteadores e as arestas representam
ligações de fibra ótica. O compartilhamento de uma fibra por mais de uma conexão se
dá por multiplexação por divisão de frequências (*Frequency Division Multiplexing*, ou FDM),
segundo a qual mais de uma conexão pode dividir um meio físico desde que elas estejam em larguras
de banda sem interseções.

A quantidade de frequências que podem ser alocadas normalmente é limitada, e, quanto menor
a concorrência em uma fibra ótica, maior a taxa de transferência de dados que uma conexão alcança.
Nesse contexto, deseja-se minimizar a quantidade de frequências alocadas nas fibras óticas da rede,
sendo dada uma lista de conexões que se deseja fazer.
Para medir isso, considera-se que a quantidade a ser minimizada é o maior número de frequências
alocadas em uma fibra qualquer da rede.

## Considerações

- Grafo não-direcionado
- Arestas sem pesos
- Requisições de conexões não podem ser repetidas
- Alocação estática: não considera comportamento temporal

## Algoritmo Exato

- Branch-and-bound
  - Limite superior (primal): menor valor entre o pior caso (grafo linha, com todas as combinações possíveis), a melhor solução encontrada até o momento e o resultado de uma heurística (a definir)
  - Limite inferior (dual): maior valor entre 1 (melhor caso corresponde a um grafo completo) e o total de frequências já alocadas (o segundo valor só vale no caso local)

  - Como verificar inviabilidade?
    - Pulei frequências demais?

## (Meta-)Heurísticas

Divisão do problema em 2:

- Roteamento:
  - Versão tonta: 1 caminho mínimo para cada par de vértices.
  - Versão mais esperta: k-menores caminhos entre todos os pares desejados
- Coloração: obtida a lista de caminhos (se houver mais de 1 por par), fazer algumas combinações e abstrair o problema para um de coloração em grafos. Um novo grafo deve ser elaborado, sendo os vértices os caminhos selecionados, e as arestas uma representação de que há pelo menos uma aresta em comum entre um par de caminhos. Executar uma heurística em cima da coloração (genético? *simulated annealing*? grasp?)

Outras propostas:

- Fluxo mínimo de múltiplas origens e múltiplos destinos. Todas as arestas possuem capacidade 1 e cada conexão é um fluxo unitário. Modificar o grafo para todas as fontes estarem ligadas a um novo nó e todos os destinos estarem ligados a um outro novo nó. Só funciona se os nós fonte não forem destinos! Pode ser necessário trocar o sentido de algumas arestas.
- *Multicommodity Flow Problem* -> isso é tão np-brabo quanto o problema original
- Algoritmo genético direto
- *Simulated Annealing* direto
- Calcular todos os (k?)caminhos mínimos entre origens e destinos e contar quantas vezes uma aresta aparece nos caminhos mínimos (*betweenness*)
- Procedimentos de caminho mínimo aplicados para caminhos máximos

## Pré-processamento

- Cortar vértices que não são origem e destino e estão isolados (grau 1 com apenas 1 dos vértices) (talvez valha a pena)

## Instâncias

- Gerar com algum modelo de grafo aleatório
  - Uma ideia é tentar G(n, p), estimando p com base em datasets grandes de redes de computadores (tem o dataset da Internet e o NSFNet)

- As duas versões do NSFNet que li nos artigos

- Como distribuir as demandas? (distribuição uniforme entre pares de vértices? priorizar distâncias longas?)
