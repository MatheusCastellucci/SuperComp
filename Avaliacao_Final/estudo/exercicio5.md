# Questão 5 – Fundamentos de Paralelismo (Total: 1.5 pontos) 

## Parte A (0,75 ponto): Descreva o modelo de memória compartilhada e memória distribuída, ressaltando seus prós e contras. Qual modelo cada uma das seguintes bibliotecas usa: OpenMP, MPI e Thrust? 

* **OpenMP**: O modelo de memória compartilhada é caracterizado por todos os processadores ou threads acessarem um único espaço de memória. Ele é eficiente para comunicação entre threads, já que não há necessidade de troca de mensagens, tornando-o ideal para arquiteturas multicore, como computadores pessoais. No entanto, esse modelo apresenta limitações em termos de escalabilidade, pois a largura de banda da memória se torna um gargalo em sistemas maiores, além de exigir mecanismos de sincronização para evitar condições de corrida.

* **MPI**: O modelo de memória distribuída é caracterizado por cada processador possuir sua própria memória, sendo necessário o uso de mensagens para comunicação entre processos. Esse modelo é ideal para sistemas distribuídos, como clusters, pois não há gargalo de largura de banda. No entanto, a comunicação por mensagens pode ser mais custosa em termos de tempo e recursos, além de exigir um maior esforço de programação para garantir a consistência dos dados.

* **Thrust**: O modelo varia. Dentro de um dispositivo como uma GPU, ele utiliza memória compartilhada, enquanto na comunicação entre CPU (host) e GPU (dispositivo), ele adota um modelo distribuído. Isso permite combinar as vantagens de ambos os modelos dependendo da parte do sistema que está sendo utilizada.

## Parte B (0,75 ponto): Explique o que é escalonamento dinâmico em OpenMP e como ele pode ser vantajoso em aplicações de processamento de dados com variabilidade de carga entre as iterações.

O escalonamento dinâmico em OpenMP é uma técnica que permite a distribuição dinâmica das iterações de um loop entre as threads disponíveis. Em vez de dividir o loop em blocos fixos e atribuí-los a cada thread, o escalonamento dinâmico divide o loop em pequenas tarefas e as distribui conforme as threads ficam disponíveis. Isso permite que as threads continuem processando iterações mesmo que outras threads estejam atrasadas ou ocupadas, evitando desbalanceamento de carga.