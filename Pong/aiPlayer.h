#pragma once

#include "gameState.h"

enum class Direction;
class GameState;

class AIPlayer
{
public:
    AIPlayer(GameState& game)
        : game(game) {}

    void update();

private:
    GameState& game;

    void move(Direction dir);
};
