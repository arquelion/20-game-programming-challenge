#pragma once
#include "cinder/gl/gl.h"

#include "intersection.h"
#include "paddle.h"
#include "view.h"

class Paddle;

enum class PowerupType : uint32_t
{
    NONE,
    LASER,
    ENLARGE,
    CATCH,
    BREAK,
    DISRUPTION,
    PLAYER,
};

class Powerup
{
public:
    Powerup(PowerupType type, glm::vec2 start);
    Powerup(Powerup&& src);

    Powerup& operator=(Powerup&& rhs) noexcept
    {
        type = std::move(rhs.type);
        id = std::move(rhs.id);
        color = std::move(rhs.color);
        center = std::move(rhs.center);
        boundingBox = std::move(rhs.boundingBox);
        return *this;
    }

    bool operator==(const Powerup& rhs)
    {
        return this->id == rhs.id;
    }

    PowerupType getType() const { return type; }
    bool hasExited() const { return (center.y - radius.y) > view.getSize().y; }
    bool isPickedUp(const Paddle& paddle) const;

    void move(float deltaSec);
    void draw() const;
    static int nextId;

private:
    PowerupType type = PowerupType::NONE;
    int id = 0;

    ci::Color color;

    glm::vec2 center;
    const glm::vec2 radius{ 3, 1 };
    const float velocity = 15.f;

    AABB boundingBox;
};
