#include "player.h"
#include <algorithm>
#include <random>

Player::Player(bool isCPU) : isCPU(isCPU) {}

void Player::addCard(const Card& card) {
    hand.push_back(card);
}

Card Player::removeCard(int index) {
    if (index < 0 || index >= static_cast<int>(hand.size())) {
        return Card(); // Return Joker as error indicator
    }
    Card card = hand[index];
    hand.erase(hand.begin() + index);
    return card;
}

Card Player::removeCardById(int suit, int rank) {
    for (auto it = hand.begin(); it != hand.end(); ++it) {
        if (static_cast<int>(it->getSuit()) == suit && it->getRank() == rank) {
            Card card = *it;
            hand.erase(it);
            return card;
        }
    }
    return Card(); // Return Joker as error indicator
}

std::vector<Card> Player::removePairs() {
    std::vector<Card> pairs;
    std::vector<bool> matched(hand.size(), false);

    for (size_t i = 0; i < hand.size(); ++i) {
        if (matched[i]) continue;

        for (size_t j = i + 1; j < hand.size(); ++j) {
            if (matched[j]) continue;

            if (hand[i].pairsWith(hand[j])) {
                matched[i] = true;
                matched[j] = true;
                pairs.push_back(hand[i]);
                pairs.push_back(hand[j]);
                break;
            }
        }
    }

    // Remove matched cards from hand (iterate in reverse to preserve indices)
    for (int i = static_cast<int>(hand.size()) - 1; i >= 0; --i) {
        if (matched[i]) {
            hand.erase(hand.begin() + i);
        }
    }

    return pairs;
}

int Player::getHandSize() const {
    return static_cast<int>(hand.size());
}

const std::vector<Card>& Player::getHand() const {
    return hand;
}

bool Player::isCPUPlayer() const {
    return isCPU;
}

bool Player::hasCards() const {
    return !hand.empty();
}

void Player::shuffleHand() {
    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(hand.begin(), hand.end(), g);
}
