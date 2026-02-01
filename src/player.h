#ifndef PLAYER_H
#define PLAYER_H

#include <vector>
#include <string>
#include "card.h"

class Player {
public:
    Player(bool isCPU = false);

    void addCard(const Card& card);
    Card removeCard(int index);
    Card removeCardById(int suit, int rank);
    std::vector<Card> removePairs();

    int getHandSize() const;
    const std::vector<Card>& getHand() const;
    bool isCPUPlayer() const;
    bool hasCards() const;
    void shuffleHand();

private:
    std::vector<Card> hand;
    bool isCPU;
};

#endif // PLAYER_H
