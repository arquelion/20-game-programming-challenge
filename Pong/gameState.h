#pragma once
#include <memory>

#include "cinder/audio/Voice.h"

#include "aiPlayer.h"
#include "arena.h"
#include "ball.h"
#include "mousePlayer.h"
#include "paddle.h"
#include "scoreboard.h"

enum class Direction
{
    UP,
    DOWN,
};

enum class Player
{
    NONE = 0,
    ONE = 1,
    TWO = 2,
};

class GameState
{
public:
    GameState() {}
    void init();

    void newGame();
    void update(float deltaSec);
    void draw();

    void movePaddle(Player player, Direction dir);

    void assignPoint(Player player);
    bool isGameOver();

    // Game elements
    std::shared_ptr<Arena> arena;

    std::shared_ptr<Ball> ball;
    std::shared_ptr<Paddle> p1Paddle;
    std::shared_ptr<Paddle> p2Paddle;

    std::shared_ptr<Scoreboard> scoreboard;

    std::unique_ptr<MousePlayer> p1;
    std::unique_ptr<AIPlayer> p2;

    ci::audio::VoiceSamplePlayerNodeRef bounceSound;

    bool isActive = true;
    bool showWinner = false;
    Player winner = Player::NONE;

private:
};
