#include <emscripten/bind.h>
#include <emscripten/val.h>
#include "game.h"

using namespace emscripten;

Game game;

void initGame() {
    game.initGame();
}

val getPlayerHand() {
    val result = val::array();
    const auto& hand = game.getPlayer(0).getHand();
    for (size_t i = 0; i < hand.size(); ++i) {
        val card = val::object();
        card.set("suit", static_cast<int>(hand[i].getSuit()));
        card.set("rank", hand[i].getRank());
        card.set("isJoker", hand[i].isJoker());
        card.set("display", hand[i].toString());
        result.call<void>("push", card);
    }
    return result;
}

int getCPUHandSize() {
    return game.getPlayer(1).getHandSize();
}

void drawFromCPU(int index) {
    game.drawCard(1, index); // Draw from CPU (player 1)
}

void cpuTurn() {
    game.cpuTurn();
}

int getGameState() {
    return static_cast<int>(game.getGameState());
}

val getDiscardedPairs() {
    val result = val::array();
    const auto& pairs = game.getDiscardedPairs();
    for (size_t i = 0; i < pairs.size(); ++i) {
        val card = val::object();
        card.set("suit", static_cast<int>(pairs[i].getSuit()));
        card.set("rank", pairs[i].getRank());
        card.set("isJoker", pairs[i].isJoker());
        card.set("display", pairs[i].toString());
        result.call<void>("push", card);
    }
    return result;
}

std::string getMessage() {
    return game.getMessage();
}

int getCurrentTurn() {
    return game.getCurrentTurn();
}

int getWinner() {
    return game.getWinner();
}

int getPlayerHandSize() {
    return game.getPlayer(0).getHandSize();
}

EMSCRIPTEN_BINDINGS(oldmaid) {
    function("initGame", &initGame);
    function("getPlayerHand", &getPlayerHand);
    function("getCPUHandSize", &getCPUHandSize);
    function("drawFromCPU", &drawFromCPU);
    function("cpuTurn", &cpuTurn);
    function("getGameState", &getGameState);
    function("getDiscardedPairs", &getDiscardedPairs);
    function("getMessage", &getMessage);
    function("getCurrentTurn", &getCurrentTurn);
    function("getWinner", &getWinner);
    function("getPlayerHandSize", &getPlayerHandSize);
}
