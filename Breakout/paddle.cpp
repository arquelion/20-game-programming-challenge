#include <algorithm>

#include "paddle.h"
#include "view.h"
#include "utils.h"

using namespace ci;

void Paddle::nudge(Ball& ball) const
{
    auto hit = boundingBox.intersectAABB(ball.boundingBox);
    if (hit)
    {
        ball.center += hit->delta + glm::vec2(epsilon) * glm::vec2(signum(hit->delta.x), signum(hit->delta.y));
        auto clampCenter = std::clamp(ball.center.x, game.arena->playArea.x1 + ball.radius, game.arena->playArea.x2 - ball.radius);
        if (clampCenter != ball.center.x)
        {
            ball.center.x = clampCenter;
            ball.center.y = (ball.center.y <= center.y)
                ? center.y - radius.y - ball.radius - epsilon
                : center.y + radius.y + ball.radius + epsilon;
        }
    }

    if (isBetween(ball.center.x, center.x - radius.x - ball.radius, center.x + radius.x + ball.radius) && isBetween(ball.center.y, center.y - radius.y - ball.radius, center.y + radius.y + ball.radius))
    {
        throw std::logic_error("ball not nudged out");
    }
}

Paddle::Paddle(GameState& game, glm::vec2 center)
    : game(game)
    , center(center)
    , initialCenter(center)
    , initialRadius(radius)
    , initialInner(0.5f, 0.3f, 0.9f)
    , initialMiddle(0.2f, 0.2f, 1.0f)
    , initialOuter(0.2f, 0.6f, 0.6f)
{
    reset();
}

void Paddle::fireLaser() const
{
    if (isLaserEnabled)
    {
        Laser left{ game, center - glm::vec2(radius.x * 0.5f, radius.y) };
        Laser right{ game, center + glm::vec2(radius.x * 0.5f, -radius.y) };
        game.lasers.emplace_back(std::move(left), std::move(right));
    }
}

void Paddle::move(glm::vec2 newPos)
{
    center.x = newPos.x;
    if (game.arena->isBonusExitOpen)
    {
        if (center.x >= game.arena->bonusExit.getX1())
        {
            wentThroughBonusExit = true;
            return;
        }
        center.x = std::max(center.x, game.arena->playArea.getX1() + radius.x);
    }
    else
    {
        center.x = std::clamp(center.x, game.arena->playArea.x1 + radius.x, game.arena->playArea.x2 - radius.x);
    }
    boundingBox.center = center;

    for (auto& ball : game.balls)
    {
        nudge(*ball);
    }
}

void Paddle::reset()
{
    center = initialCenter;
    radius = initialRadius;
    inner = initialInner;
    middle = initialMiddle;
    outer = initialOuter;
    boundingBox.center = center;
    boundingBox.radius = radius;
    isCatchEnabled = false;
    isLaserEnabled = false;
    wentThroughBonusExit = false;
}

void Paddle::draw() const
{
    gl::color(outer);
    gl::drawSolidRect(view.getRect(center - radius, center + radius));
    gl::color(middle);
    gl::drawSolidRect(view.getRect(center - glm::vec2(radius.x * 0.8f, radius.y), center + glm::vec2(radius.x * 0.8f, radius.y)));
    gl::color(inner);
    gl::drawSolidRect(view.getRect(center - glm::vec2(radius.x * 0.5f, radius.y), center + glm::vec2(radius.x * 0.5f, radius.y)));
}
