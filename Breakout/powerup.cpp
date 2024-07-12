#include "paddle.h"
#include "powerup.h"
#include "view.h"

using namespace ci;

int Powerup::nextId = 1;

Powerup::Powerup(PowerupType type, glm::vec2 start)
    : type(type)
    , center(start)
{
    switch (type)
    {
    case PowerupType::LASER:
        color = Color(1.f, 0.f, 0.f);
        break;
    case PowerupType::ENLARGE:
        color = Color(0.f, 0.f, 1.f);
        break;
    case PowerupType::CATCH:
        color = Color(0.f, 1.f, 0.f);
        break;
    case PowerupType::BREAK:
        color = Color(1.f, 0.2f, 0.8f);
        break;
    case PowerupType::DISRUPTION:
        color = Color(0.2f, 0.8f, 1.f);
        break;
    case PowerupType::PLAYER:
        color = Color(0.5f, 0.5f, 0.5f);
        break;
    default:
        break;
    }

    boundingBox.center = center;
    boundingBox.radius = radius;

    id = nextId++;
}

Powerup::Powerup(Powerup&& src)
{
    *this = std::move(src);
}

bool Powerup::isPickedUp(const Paddle& paddle) const
{
    auto hit = paddle.boundingBox.intersectAABB(this->boundingBox);
    return hit.has_value();
}

void Powerup::move(float deltaSec)
{
    center.y += velocity * deltaSec;
    boundingBox.center = center;
}

void Powerup::draw() const
{
    gl::color(color);
    gl::drawSolidRoundedRect(view.getRect(center - radius, center + radius), view.calcScaledValue(0.5f));
}
