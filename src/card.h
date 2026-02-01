#ifndef CARD_H
#define CARD_H

#include <string>

enum class Suit {
    HEARTS = 0,
    DIAMONDS = 1,
    CLUBS = 2,
    SPADES = 3,
    JOKER = 4
};

class Card {
public:
    Card(Suit suit, int rank);
    Card(); // Default constructor for Joker

    Suit getSuit() const;
    int getRank() const;
    bool isJoker() const;
    std::string toString() const;

    bool operator==(const Card& other) const;
    bool pairsWith(const Card& other) const;

private:
    Suit suit;
    int rank; // 1-13 (A-K), 0 for Joker
};

#endif // CARD_H
