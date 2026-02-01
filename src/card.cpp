#include "card.h"

Card::Card(Suit suit, int rank) : suit(suit), rank(rank) {}

Card::Card() : suit(Suit::JOKER), rank(0) {}

Suit Card::getSuit() const {
    return suit;
}

int Card::getRank() const {
    return rank;
}

bool Card::isJoker() const {
    return suit == Suit::JOKER;
}

std::string Card::toString() const {
    if (isJoker()) {
        return "Joker";
    }

    std::string suitStr;
    switch (suit) {
        case Suit::HEARTS:   suitStr = "H"; break;
        case Suit::DIAMONDS: suitStr = "D"; break;
        case Suit::CLUBS:    suitStr = "C"; break;
        case Suit::SPADES:   suitStr = "S"; break;
        default:             suitStr = "?"; break;
    }

    std::string rankStr;
    switch (rank) {
        case 1:  rankStr = "A"; break;
        case 11: rankStr = "J"; break;
        case 12: rankStr = "Q"; break;
        case 13: rankStr = "K"; break;
        default: rankStr = std::to_string(rank); break;
    }

    return suitStr + rankStr;
}

bool Card::operator==(const Card& other) const {
    return suit == other.suit && rank == other.rank;
}

bool Card::pairsWith(const Card& other) const {
    if (isJoker() || other.isJoker()) {
        return false;
    }
    return rank == other.rank;
}
