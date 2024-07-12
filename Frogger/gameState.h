#pragma once
#include "game.h"

#include "car.h"
#include "frog.h"

#include "arena.h"
#include "clock.h"
#include "scoreboard.h"
#include "textBox.h"

enum class Direction
{
    UP,
    DOWN,
    LEFT,
    RIGHT,
};

class GameState
{
public:
    GameState() {}
    void init();

    void newGame();
    bool isGameOver() const;
    void loseLife();

    void update(float deltaSec);
    void draw() const;

    // Game elements
    std::shared_ptr<Arena> arena;
    std::shared_ptr<Scoreboard> scoreboard;

    std::shared_ptr<Frog> frog;
    bool extraLifeGained = false;
    int livesRemaining = 7;
    int frogsRemaining = 1;

    std::vector<int>highScores{ 0, 0, 0, 0, 0 };

    bool isActive = true;

    Clock clock;
    Clock::time_point frogTimeLimit;
    Clock::duration timePerFrog = std::chrono::seconds(60);

private:
    enum class LevelState
    {
        LOAD,
        READY,
        PLAY,
    } levelState;

    Clock::time_point transitionTime;
};
