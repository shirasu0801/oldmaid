#ifndef GAME_H
#define GAME_H

#include <vector>
#include <string>
#include "card.h"
#include "player.h"

enum class GameState {
    NOT_STARTED = 0,
    PLAYER_TURN = 1,
    CPU_TURN = 2,
    GAME_OVER = 3
};

class Game {
public:
    Game();

    void initGame();
    void drawCard(int fromPlayer, int cardIndex);
    int cpuSelectCard();
    void cpuTurn();
    bool checkGameEnd();
    int getWinner() const; // -1: not ended, 0: player wins, 1: CPU wins

    GameState getGameState() const;
    const Player& getPlayer(int index) const;
    const std::vector<Card>& getDiscardedPairs() const;
    std::string getMessage() const;
    int getCurrentTurn() const;

private:
    Player players[2]; // 0: Human, 1: CPU
    GameState gameState;
    int currentTurn; // 0 or 1
    std::vector<Card> discardedPairs;
    std::string message;

    void createDeck(std::vector<Card>& deck);
    void shuffleDeck(std::vector<Card>& deck);
    void dealCards(std::vector<Card>& deck);
    void removeInitialPairs();
    void switchTurn();
};

#endif // GAME_H
