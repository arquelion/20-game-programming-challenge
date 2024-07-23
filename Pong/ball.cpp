#include <algorithm>

#include "ball.h"
#include "utils.h"
#include "view.h"

using namespace ci;

Ball::Ball(GameState& game)
    : game(game)
{
    reset();
}

void Ball::reset()
{
    auto nextX = (lastScored == Player::ONE) ? 1.f : -1.f;
    auto netHeight = game.arena->netBot.y - game.arena->netTop.y;
    pos = { 0.5f - nextX * 0.1f, std::max(game.arena->netTop.y + 0.1f, std::min(game.arena->netBot.y - 0.1f, game.arena->netTop.y + getRandomNum<float>() * netHeight)) };
    auto angle = lerp(-M_PI / 3, M_PI / 3, getRandomNum<float>());
    velocity = { nextX * cos(angle) , sin(angle) };
    velocity *= 0.5f;
}

void Ball::updateVelocity(CollisionObject object)
{
    switch (object)
    {
    case CollisionObject::P1PADDLE:
        fprintf(logFile, "Collision: P1, Frame %d\n", frameCount);
        velocity.y += 0.1f * game.p1Paddle->velocity;
        velocity.x = -velocity.x;
        break;
    case CollisionObject::P2PADDLE:
        fprintf(logFile, "Collision: P2, Frame %d\n", frameCount);
        velocity.y += 0.1f * game.p2Paddle->velocity;
        velocity.x = -velocity.x;
        break;
    case CollisionObject::WALL:
        velocity.y = -velocity.y;
        break;
    default:
        throw std::exception();
    }
    game.bounceSound->start();
}

void Ball::move(float deltaSec)
{
    pos += velocity * deltaSec;
    auto minY = game.arena->upperWallLeft.y + game.arena->arenaStrokeWidth / 2 + radius;
    auto maxY = game.arena->lowerWallLeft.y - game.arena->arenaStrokeWidth / 2 - radius;
    if (pos.y < minY) // upper wall
    {
        pos.y = minY + (minY - pos.y);
        updateVelocity(CollisionObject::WALL);
    }
    else if (pos.y > maxY) // lower wall
    {
        pos.y = maxY - (pos.y - maxY);
        updateVelocity(CollisionObject::WALL);
    }
    
    if (pos.x < game.arena->lowerWallLeft.x)
    {
        if (game.isActive)
        {
            fprintf(logFile, "Score: P2\n");
            game.assignPoint(Player::TWO);
            lastScored = Player::TWO;
            reset();
        }
        else
        {
            updateVelocity(CollisionObject::P1PADDLE);
        }
    }
    else if (pos.x > game.arena->lowerWallRight.x)
    {
        if (game.isActive)
        {
            fprintf(logFile, "Score: P1\n");
            game.assignPoint(Player::ONE);
            lastScored = Player::ONE;
            reset();
        }
        else
        {
            updateVelocity(CollisionObject::P1PADDLE);
        }
    }

    if (game.isActive)
    {
        if (isBetween(pos.x, game.p1Paddle->topLeft.x, game.p1Paddle->botRight.x)
            && isBetween(pos.y, game.p1Paddle->topLeft.y, game.p1Paddle->botRight.y)) // left paddle
        {
            pos.x = game.p1Paddle->botRight.x + (game.p1Paddle->botRight.x - pos.x);
            updateVelocity(CollisionObject::P1PADDLE);
        }
        else if (isBetween(pos.x, game.p2Paddle->topLeft.x, game.p2Paddle->botRight.x)
            && isBetween(pos.y, game.p2Paddle->topLeft.y, game.p2Paddle->botRight.y)) // right paddle
        {
            pos.x = game.p2Paddle->topLeft.x - (pos.x - game.p2Paddle->topLeft.x);
            updateVelocity(CollisionObject::P2PADDLE);
        }
    }
}

void Ball::draw() const
{
    gl::drawSolidCircle(view.getViewVec2(pos), view.calcScaledValue(radius));
}
