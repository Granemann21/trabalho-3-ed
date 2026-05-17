#include <iostream>
#include <chrono>
#include <string>
#include "Matchmaking.hpp"

void imprimirELiberar(Player* arr, int n, std::string titulo) {
    std::cout << titulo << ":" << std::endl;
    if (arr == nullptr || n == 0) {
        std::cout << "(empty)" << std::endl;
    } else {
        for (int i = 0; i < n; i++) {
            std::cout << "[" << arr[i].getId() << " | " << arr[i].getName() << " | " 
                      << arr[i].getScore() << " | " << arr[i].getTimestamp() << "]" << std::endl;
        }
        delete[] arr; 
    }
    std::cout << std::endl;
}

void testFuncional() {
    std::cout << "========== TESTES FUNCIONAIS ==========" << std::endl;
    
    Matchmaking* matchmaking = new Matchmaking();

    // 1. Teste de Inserção
    matchmaking->insert(Player(1, "Ana", 1000, 5));
    matchmaking->insert(Player(2, "Bruno", 900, 2));
    matchmaking->insert(Player(3, "Carla", 1000, 1));
    matchmaking->insert(Player(4, "Diego", 900, 4));

    std::cout << "\n--- Estado Inicial (Apos Insercoes) ---" << std::endl;
    matchmaking->printWaitingPlayers();

    // 2. Teste de Ordenação com Empate
    // Esperado: Bruno(900,2), Diego(900,4), Carla(1000,1), Ana(1000,5)
    matchmaking->sortByScoreMerge();
    std::cout << "\n--- Apos Merge Sort (Criterio: Score, entao Timestamp) ---" << std::endl;
    matchmaking->printWaitingPlayers();

    // 3. Formação de Grupo - Falha
    int n;
    Player* g1 = matchmaking->formGroup(3, 50, &n);
    std::cout << "\n--- Tentativa de Grupo (3 jogadores, delta 50) ---" << std::endl;
    imprimirELiberar(g1, n, "Resultado do Grupo");

    // 4. Formação de Grupo - Sucesso
    // O primeiro grupo válido de 2 jogadores com delta 10 (Bruno e Diego)
    Player* g2 = matchmaking->formGroup(2, 10, &n);
    std::cout << "\n--- Tentativa de Grupo (2 jogadores, delta 10) ---" << std::endl;
    imprimirELiberar(g2, n, "Resultado do Grupo");

    std::cout << "\n--- Fila apos remocao do grupo formado ---" << std::endl;
    matchmaking->printWaitingPlayers();

    // 5. Remoção por ID
    matchmaking->removePlayer(1); // Remove Ana
    std::cout << "\n--- Apos remover ID 1 (Ana) ---" << std::endl;
    matchmaking->printWaitingPlayers();

    // 6. Recuperação de dados via getWaitingPlayers
    int total;
    Player* copia = matchmaking->getWaitingPlayers(&total);
    imprimirELiberar(copia, total, "Copia da Fila Atual (via getWaitingPlayers)");

    delete matchmaking;
    std::cout << std::endl;
}

void testDesempenho() {
    std::cout << "\n========== TABELA DE DESEMPENHO ==========" << std::endl;
    std::cout << "N\t\t| Insertion Sort (ms)\t| Merge Sort (ms)" << std::endl;
    std::cout << "----------------------------------------------------------" << std::endl;

    int tamanhos[] = {100, 500, 1000, 5000, 10000};

    for (int n : tamanhos) {
        Matchmaking* mI = new Matchmaking();
        Matchmaking* mM = new Matchmaking();

        // Geração determinística de dados
        for (int i = 0; i < n; i++) {
            int score = (i * 13) % 5000; 
            Player p(i, "Bot", score, i);
            mI->insert(p);
            mM->insert(p);
        }

        // Medição do Insertion Sort
        auto startI = std::chrono::high_resolution_clock::now();
        mI->sortByScoreInsertion();
        auto endI = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double, std::milli> diffI = endI - startI;

        // Medição do Merge Sort
        auto startM = std::chrono::high_resolution_clock::now();
        mM->sortByScoreMerge();
        auto endM = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double, std::milli> diffM = endM - startM;

        std::cout << n << "\t\t| " << diffI.count() << "\t\t\t| " << diffM.count() << std::endl;

        delete mI;
        delete mM;
    }
}

int main() {
    testFuncional();
    testDesempenho();
    return 0;
}