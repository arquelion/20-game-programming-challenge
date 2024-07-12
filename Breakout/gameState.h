#pragma once
#include <memory>
#include <utility>
#include <vector>

#include "cinder/audio/Voice.h"

#include "arena.h"
#include "ball.h"
#include "clock.h"
#include "laser.h"
#include "paddle.h"
#include "powerup.h"
#include "scoreboard.h"
#include "textBox.h"

class Arena;
class Laser;
class Paddle;
class Scoreboard;

enum class CollisionObject
{
    NONE,
    BRICK,
    EXIT,
    PADDLE,
    WALL,
};

class GameState
{
public:
    GameState() : readyMsg(5.f, { 50, 50 }) {}
    void init();

    void newGame();
    bool isGameOver();
    void loseLife();

    void update(float deltaSec);
    void draw();

    // Game elements
    std::shared_ptr<Arena> arena;
    std::shared_ptr<Scoreboard> scoreboard;

    std::vector<std::shared_ptr<Ball>> balls;
    std::shared_ptr<Paddle> paddle;

    bool arePowerupsEnabled = true;
    PowerupType activePowerup;
    std::vector<Powerup> fallingPowerups;
    int bricksBeforeNextPowerup = 0;

    std::vector<std::pair<Laser, Laser>> lasers;

    std::vector<int>highScores{ 0, 0, 0, 0, 0 };

    bool isActive = false;
    int currentLevel = 1;
    int levelsRemaining = 2;

    Clock clock;

    ci::audio::VoiceSamplePlayerNodeRef bounceSound;

private:
    enum class LevelState
    {
        LOAD,
        READY,
        PLAY,
    } levelState;

    Clock::time_point transitionTime;

    ::TextBox readyMsg;

    void activateNewPowerup(PowerupType newType);
    void deactivatePowerup();
};
