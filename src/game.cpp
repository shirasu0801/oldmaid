#include "game.h"
#include <algorithm>
#include <random>

Game::Game() : gameState(GameState::NOT_STARTED), currentTurn(0) {
    players[0] = Player(false); // Human
    players[1] = Player(true);  // CPU
}

void Game::initGame() {
    // Reset players
    players[0] = Player(false);
    players[1] = Player(true);
    discardedPairs.clear();

    // Create and shuffle deck
    std::vector<Card> deck;
    createDeck(deck);
    shuffleDeck(deck);

    // Deal cards
    dealCards(deck);

    // Remove initial pairs
    removeInitialPairs();

    // Shuffle hands so positions are random
    players[0].shuffleHand();
    players[1].shuffleHand();

    // Start game - player goes first
    currentTurn = 0;
    gameState = GameState::PLAYER_TURN;
    message = "ゲーム開始！CPUの手札からカードを選んでください。";
}

void Game::createDeck(std::vector<Card>& deck) {
    // Add 52 regular cards
    for (int suit = 0; suit < 4; ++suit) {
        for (int rank = 1; rank <= 13; ++rank) {
            deck.push_back(Card(static_cast<Suit>(suit), rank));
        }
    }
    // Add Joker
    deck.push_back(Card());
}

void Game::shuffleDeck(std::vector<Card>& deck) {
    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(deck.begin(), deck.end(), g);
}

void Game::dealCards(std::vector<Card>& deck) {
    for (size_t i = 0; i < deck.size(); ++i) {
        players[i % 2].addCard(deck[i]);
    }
}

void Game::removeInitialPairs() {
    auto pairs0 = players[0].removePairs();
    auto pairs1 = players[1].removePairs();

    discardedPairs.insert(discardedPairs.end(), pairs0.begin(), pairs0.end());
    discardedPairs.insert(discardedPairs.end(), pairs1.begin(), pairs1.end());
}

void Game::drawCard(int fromPlayer, int cardIndex) {
    if (gameState != GameState::PLAYER_TURN && gameState != GameState::CPU_TURN) {
        return;
    }

    int toPlayer = 1 - fromPlayer;

    if (cardIndex < 0 || cardIndex >= players[fromPlayer].getHandSize()) {
        return;
    }

    Card card = players[fromPlayer].removeCard(cardIndex);
    players[toPlayer].addCard(card);

    // Check for pairs
    auto pairs = players[toPlayer].removePairs();
    discardedPairs.insert(discardedPairs.end(), pairs.begin(), pairs.end());

    std::string cardName = card.isJoker() ? "ジョーカー" : card.toString();

    if (toPlayer == 0) {
        message = "あなたは " + cardName + " を引きました。";
    } else {
        message = "CPUは " + cardName + " を引きました。";
    }

    if (!pairs.empty()) {
        message += " ペアができました！";
    }

    // Shuffle the drawer's hand
    players[toPlayer].shuffleHand();

    // Check game end
    if (checkGameEnd()) {
        gameState = GameState::GAME_OVER;
        int winner = getWinner();
        if (winner == 0) {
            message = "おめでとう！あなたの勝ちです！";
        } else {
            message = "残念！CPUの勝ちです...";
        }
    } else {
        switchTurn();
    }
}

int Game::cpuSelectCard() {
    if (players[0].getHandSize() == 0) {
        return -1;
    }

    std::random_device rd;
    std::mt19937 g(rd());
    std::uniform_int_distribution<> dis(0, players[0].getHandSize() - 1);
    return dis(g);
}

void Game::cpuTurn() {
    if (gameState != GameState::CPU_TURN) {
        return;
    }

    int cardIndex = cpuSelectCard();
    if (cardIndex >= 0) {
        drawCard(0, cardIndex); // CPU draws from player (0)
    }
}

bool Game::checkGameEnd() {
    return !players[0].hasCards() || !players[1].hasCards();
}

int Game::getWinner() const {
    if (!players[0].hasCards() && !players[1].hasCards()) {
        return -1; // Should not happen
    }
    if (!players[0].hasCards()) {
        return 0; // Player wins (no cards left)
    }
    if (!players[1].hasCards()) {
        return 1; // CPU wins (no cards left)
    }
    return -1; // Game not ended
}

GameState Game::getGameState() const {
    return gameState;
}

const Player& Game::getPlayer(int index) const {
    return players[index];
}

const std::vector<Card>& Game::getDiscardedPairs() const {
    return discardedPairs;
}

std::string Game::getMessage() const {
    return message;
}

int Game::getCurrentTurn() const {
    return currentTurn;
}

void Game::switchTurn() {
    currentTurn = 1 - currentTurn;
    if (currentTurn == 0) {
        gameState = GameState::PLAYER_TURN;
    } else {
        gameState = GameState::CPU_TURN;
    }
}
