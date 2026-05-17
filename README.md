# Matchmaking System

## Descrição do Trabalho
Este projeto consiste na implementação de um sistema de **Matchmaking** simplificado para a organização de partidas em jogos online competitivos. O sistema gerencia jogadores em uma fila de espera, agrupando-os automaticamente em equipes equilibradas assim que os critérios de proximidade de nível de habilidade (*score*) são atingidos.

O diferencial deste sistema é que ele tira proveito da **ordenação prévia** dos jogadores na fila para otimizar a busca por grupos válidos e utiliza a ordem de chegada (*timestamp*) como critério de desempate, garantindo a formação justa de partidas conforme as regras especificadas no enunciado.

---

## Instruções de Compilação e Execução
O projeto foi desenvolvido de forma modular em C++, sem a utilização de estruturas prontas ou algoritmos de ordenação da STL, exigindo a compilação de todas as classes que compõem o sistema.

Para compilar o projeto completo, execute o seguinte comando no terminal:
```bash
g++ main.cpp Matchmaking.cpp Player.cpp -o matchmaking
```

Para executar, siga o seguinte passo, verificando qual o seu ambiente:

### Windows (PowerShell ou CMD):
```
.\order_book.exe
```

### Linux/MacOS (Terminal):
```
chmod +x order_book
./order_book
```

---
## Organização Interna e Arquitetura

1. Estrutura de Dados

    `Player`: Classe que encapsula os dados individuais de cada jogador, armazenando seu identificador único, nome, score e timestamp.

    `Matchmaking`: Gerenciador central responsável por armazenar os jogadores em espera dentro de um array estático de tamanho fixo, delimitado pela constante `MAX_PLAYERS = 100000`.

2. Funcionamento do Sistema

    A lógica de gerenciamento e pareamento segue o fluxo:

    * **Inserção:** O método `insert` adiciona novos jogadores sempre ao final da estrutura estática interna, incrementando o contador `size`.
    * **Ordenação:** Realizada manualmente pelos métodos `sortByScoreInsertion` ou `sortByScoreMerge`. Organiza a fila por menor score e, em caso de igualdade, pelo menor timestamp.
    * **Formação de Grupo:** O método `formGroup` analisa uma janela deslizante consecutiva de tamanho `groupSize`. Devido à ordenação prévia, valida em tempo constante $O(1)$ se a diferença de score entre as extremidades da janela atende ao `delta` estipulado. O primeiro grupo válido encontrado é extraído e os jogadores remanescentes sofrem um deslocamento à esquerda dos elementos restantes.

3. Gerenciamento de Memória

    * **Array Estático:** A fila principal de matchmaking é alocada de forma estática dentro do gerenciador, mitigando custos de alocação de nós individuais durante as inserções.
    * **Retorno por Cópia:** Os métodos `formGroup` e `getWaitingPlayers` geram e retornam novos arrays contendo cópias dos registros alocados dinamicamente no heap via `new[]`.
    * **Responsabilidade de Liberação:** A desalocação completa de toda memória dinâmica retornada por essas funções através do operador `delete[]` é de responsabilidade estrita da função chamadora, prevenindo a ocorrência de memory leaks.

---
## Execução dos Testes

O arquivo `main.cpp` contém os testes já automatizados. Para executar as validações funcionais e a tabela de desempenho, rode o binário gerado após a compilação:

* **Testes Funcionais (`testFuncional`):** Executa automaticamente todos os cenários de exemplo do enunciado (inserções, ordenação com empate por timestamp, falha e sucesso na formação de grupos por janela deslizante e remoção de jogadores).
* **Testes de Desempenho (`testDesempenho`):** Gera dados determinísticos desordenados e mede o tempo de execução (em milissegundos) via `<chrono>` do *Insertion Sort* e *Merge Sort* para diferentes tamanhos de entrada ($N \in \{100, 500, 1000, 5000, 10000\}$).

Comandos para execução:

```bash
# No Linux / macOS:
./matchmaking

# No Windows (PowerShell ou CMD):
.\matchmaking.exe
```

---
## Autores
* **Luis Eduardo Weigert Weiss**
* **Murilo Granemann de Souza**

---
*Projeto desenvolvido para o Trabalho 3 da disciplina de Estrutura de Dados - Maio de 2026.*
