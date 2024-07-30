#pragma once
#include "game.h"

#include "car.h"

#include "arena.h"
#include "clock.h"
#include "scoreboard.h"
#include "textBox.h"

class GameState
{
public:
    GameState() {}
    void init();

    void newGame();
    bool isGameOver() const;
    
    void update(float deltaSec);
    void draw() const;

    // Game elements
    std::shared_ptr<Arena> arena;
    std::shared_ptr<Scoreboard> scoreboard;

    bool isActive = true;

    Clock clock;

private:
    enum class LevelState
    {
        LOAD,
        READY,
        PLAY,
    } levelState;

    //int currentLevel = 1;

    Clock::time_point transitionTime;
};
