#include "aiPlayer.h"
#include "utils.h"

void AIPlayer::update()
{
    auto& paddle = game.p2Paddle;
    auto& ball = game.ball;

    if (ball->pos.x <= 0.5f || ball->velocity.x < 0)
    {
        return;
    }

    if (ball->pos.y > paddle->botRight.y)
    {
        move(Direction::DOWN);
    }
    else if (ball->pos.y < paddle->topLeft.y)
    {
        move(Direction::UP);
    }
}

void AIPlayer::move(Direction dir)
{
    if (getRandomNum<float>() < 0.33)
    {
        return;
        if (dir == Direction::UP)
        {
            dir = Direction::DOWN;
        }
        else
        {
            dir = Direction::UP;
        }
    }

    if (dir == Direction::UP)
    {
        game.p2Paddle->keyMoveUp();
    }
    else
    {
        game.p2Paddle->keyMoveDown();
    }
}
