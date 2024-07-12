#include <chrono>
#include <thread>

#include "cinder/gl/gl.h"

#include "gameState.h"
#include "view.h"

using namespace ci;

void GameState::init()
{
    arena = std::make_shared<Arena>();

    ball = std::make_shared<Ball>(*this);
    p1Paddle = std::make_shared<Paddle>(arena, 0.05f, 0.55f);
    p2Paddle = std::make_shared<Paddle>(arena, 0.94f, 0.55f);

    scoreboard = std::make_shared<Scoreboard>(glm::vec2(0.5f, 0.05f));

    p1 = std::make_unique<MousePlayer>(*p1Paddle);
    p2 = std::make_unique<AIPlayer>(*this);
}

void GameState::newGame()
{
    scoreboard->reset();
    ball->reset();
    p1Paddle->reset();
    p2Paddle->reset();
    isActive = true;
}

void GameState::update(float deltaSec)
{
    if (isActive)
    {
        scoreboard->setFontSize();
        p1Paddle->updateVelocity();
        p2Paddle->updateVelocity();
        p2->update();
    }
    
    ball->move(deltaSec);
}

void GameState::draw()
{
    arena->draw();
    ball->draw();

    if (isActive)
    {
        p1Paddle->draw();
        p2Paddle->draw();

        scoreboard->draw();
    }
}

void GameState::movePaddle(Player player, Direction dir)
{
    switch (dir)
    {
    case Direction::UP:
        if (player == Player::ONE)
        {
            p1Paddle->keyMoveUp();
        }
        else
        {
            p2Paddle->keyMoveUp();
        }
        break;
    case Direction::DOWN:
        if (player == Player::ONE)
        {
            p1Paddle->keyMoveDown();
        }
        else
        {
            p2Paddle->keyMoveDown();
        }
        break;
    default:
        return;
    }
}

void GameState::assignPoint(Player player)
{
    scoreboard->incrementScore(int(player));
}

bool GameState::isGameOver()
{
    auto scores = scoreboard->getScores();
    if ((scores[0] > 10 || scores[1] > 10) && (abs(scores[0] - scores[1]) >= 2))
    {
        winner = (scores[0] > scores[1]) ? Player::ONE : Player::TWO;
        return true;
    }
    return false;
}
