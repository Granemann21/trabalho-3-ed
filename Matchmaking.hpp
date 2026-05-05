#ifndef MATCHMAKING_HPP
#define MATCHMAKING_HPP
#include "Player.hpp"

const int MAX_PLAYERS = 100000;


class Matchmaking {

private:

    Player players[MAX_PLAYERS];
    int size;

    Player* merge(Player arr1[], int n1, Player arr2[], int n2);
    Player* mergeSort(Player arr[], int n);

public:

    Matchmaking();
    ~Matchmaking();

    bool insert(Player player);
    bool removePlayer(int id);

    void sortByScoreInsertion();
    void sortByScoreMerge();

    Player* formGroup(int groupSize, int delta, int* n);

    Player* getWaitingPlayers(int* n);

    void printWaitingPlayers();

    // Outros métodos auxiliares, se necessário
};

#endif