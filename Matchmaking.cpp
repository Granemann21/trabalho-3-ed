#include "Matchmaking.hpp"
#include <iostream>

Matchmaking::Matchmaking(){
    this->size = 0;
}

Matchmaking::~Matchmaking(){}

bool Matchmaking::insert(Player player){
    // Se não tiver espaço, retorna false
    if (size >= MAX_PLAYERS){ 
        return false;
    }
    
    // Adiciona o player e aumenta o size
    this->players[size] = player;
    size++;
    return true;
}

bool Matchmaking::removePlayer(int id){
    if (this->size == 0) return false;

    for (int i = 0; i < size; i++){
        if (this->players[i].getId() == id){
            for (int j = i; j < size; j++) {
                // Dando um shift pra esquerda nos players que estavam à direita do que foi removido
                this->players[j] = this->players[j+1];
            }
            this->size--;
            return true;
        }
    }
    // Não achou o id
    return false;
}

void Matchmaking::sortByScoreInsertion(){

    for (int i = 1; i < this->size; i++){
        Player p_current = this->players[i];
        int j = i - 1;

        // Enquanto o score de j for menor que o de i ou se forem iguais mas o timestamp de j é menor, continua
        while (j >= 0 && (this->players[j].getScore() > p_current.getScore() || 
                 (this->players[j].getScore() == p_current.getScore() && 
                  this->players[j].getTimestamp() > p_current.getTimestamp()))) {

            this->players[j + 1] = this->players[j];
            j--;
            }
        this->players[j + 1] = p_current;
    }
}

Player* Matchmaking::merge(Player arr1[], int n1, Player arr2[], int n2){
    Player* mArr = new Player[n1 + n2];

    int i = 0;
    int j = 0;

    while (i < n1 && j < n2){
        if (arr1[i].getScore() < arr2[j].getScore()){
            mArr[i + j] = arr1[i];
            i++;
        }
        else if (arr1[i].getScore() > arr2[j].getScore()){
            mArr[i + j] = arr2[j];
            j++;
        } else {
            if (arr1[i].getTimestamp() < arr2[j].getTimestamp()){
               mArr[i + j] = arr1[i];
                i++; 
            }
            else {
                mArr[i + j] = arr2[j];
                j++;
            }
        }
    }

    // Preenche com o que restou (se restou) do outro array 
    while (i < n1) {
        mArr[i + j] = arr1[i];
        i++;
    }

    while (j < n2) {
        mArr[i + j] = arr2[j];
        j++;
    }

    return mArr;
}

Player* Matchmaking::mergeSort(Player arr[], int n){
    if (n <= 0){
        return nullptr;
    }

    if (n == 1){
        Player* single = new Player[1];
        single[0] = arr[0];
        return single;
    }

    int mid = n/2;

    Player* left = mergeSort(arr, mid);
    Player* right = mergeSort(arr + mid, n - mid);

    Player* sorted = merge(left, mid, right, n - mid);

    delete[] left;
    delete[] right;
    
    return sorted;
}

void Matchmaking::sortByScoreMerge(){
    // Se o array tiver vazio pode dar problema
    if (this->size == 0) return;

    Player* players = this->mergeSort(this->players, this->size);
    for (int i = 0; i < this->size; i++){
        this->players[i] = players[i]; 
    }
    delete[] players;
}

Player* Matchmaking::formGroup(int groupSize, int delta, int* n){

    int i = 0;
    // Olhamos em uma janela
    while(i + groupSize - 1 < this->size){
        // Caso esteja no intervalo
        if (this->players[i + groupSize - 1].getScore() - this->players[i].getScore() <= delta){
            Player* groupPlayers = new Player[groupSize];

            // Preenche o array
            for(int j = 0; j < groupSize; j++){
                groupPlayers[j] = this->players[i + j];
            }

            // Somente sobreescrevemos os valores que ficaram à direita no lugar dos antigos valores
            for (int j = i; j < this->size - groupSize; j++) {
                this->players[j] = this->players[j + groupSize];
            }

            // Calcula o novo tamanho do array
            this->size -= groupSize;

            *n = groupSize;
            return groupPlayers;

        }
        i++;
    }
    
    // Se passou o while inteiro e não retornou, quer dizer que não foi possível formar grupo
    *n = 0;
    return nullptr;
}

Player* Matchmaking::getWaitingPlayers(int* n){
    if (this->size == 0) {
        *n = 0;
        return nullptr;
    }
    *n = this->size;
    Player* copy = new Player[this->size];
    for (int i = 0; i < this->size; i++) {
        copy[i] = this->players[i];
    }
    return copy;
}

void Matchmaking::printWaitingPlayers() {
    std::cout << "Waiting Players:" << std::endl;
    if (this->size == 0) {
        std::cout << "(empty)" << std::endl;
        return;
    }
    for (int i = 0; i < this->size; i++) {
        std::cout << "[" << this->players[i].getId() << " | " 
                  << this->players[i].getName() << " | " 
                  << this->players[i].getScore() << " | " 
                  << this->players[i].getTimestamp() << "]" << std::endl;
    }
}