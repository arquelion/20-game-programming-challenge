#include <cmath>

#include "brick.h"
#include "view.h"
#include "utils.h"

using namespace ci;

Brick::Brick(GameState& game, BrickColor color, glm::vec2 center, int level)
    : game(game)
    , color(color)
    , center(center)
{
    if (color == BrickColor::SILVER)
    {
        hitsRemaining = 2 + level / 8;
    }
}

void Brick::hit()
{
    --hitsRemaining;
    if (color != BrickColor::GOLD && hitsRemaining <= 0)
    {
        if (game.arePowerupsEnabled)
        {
            --game.bricksBeforeNextPowerup;
            if (game.bricksBeforeNextPowerup == 0)
            {
                auto type = static_cast<PowerupType>(std::ceil(getRandomNum<float>() * 6.f));
                game.fallingPowerups.emplace_back(type, center);
                game.bricksBeforeNextPowerup = (int)std::ceil(getRandomNum<float>() * 15.f);
            }
        }

        isActive = false;
        radius = { 0, 0 };
        center = { -1, -1 };
        boundingBox = { center, radius };
    }
}

void Brick::draw()
{
    gl::color(Color::black());
    gl::drawSolidRect(view.getRect(center - radius, center + radius));

    switch (color)
    {
    case BrickColor::WHITE:
        gl::color(Color::white());
        break;
    case BrickColor::ORANGE:
        gl::color(Color(1.f, 0.647f, 0.f));
        break;
    case BrickColor::CYAN:
        gl::color(Color(0.f, 1.f, 1.f));
        break;
    case BrickColor::GREEN:
        gl::color(Color(0.f, 1.0f, 0.f));
        break;
    case BrickColor::RED:
        gl::color(Color(1.f, 0.f, 0.f));
        break;
    case BrickColor::BLUE:
        gl::color(Color(0.f, 0.f, 1.f));
        break;
    case BrickColor::MAGENTA:
        gl::color(Color(1.f, 0.f, 1.f));
        break;
    case BrickColor::YELLOW:
        gl::color(Color(1.f, 1.f, 0.f));
        break;
    case BrickColor::SILVER:
        gl::color(Color(0.753f, 0.753f, 0.753f));
        break;
    case BrickColor::GOLD:
        gl::color(Color(1.f, 0.843f, 0.f));
        break;
    default:
        return;
    }

    gl::drawSolidRect(view.getRect(center - radius + glm::vec2(0.25f), center + radius - glm::vec2(0.25f)));
}
