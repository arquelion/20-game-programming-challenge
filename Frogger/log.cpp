#include "precomp.h"

#include "log.h"

#include "gameState.h"

using namespace ci;

Log::Log(LogSize size, float speed)
    : size(size)
{
    // TODO: load textures
    glm::vec2 radius;
    float initialY = 0.f;
    switch (size)
    {
    case LogSize::MEDIUM:
        radius = { 25.f, 3.f };
        initialY = game.arena->water.getTopLeft().y + 5.f;
        break;
    case LogSize::LARGE:
        radius = { 35.f, 3.f };
        initialY = game.arena->water.getTopLeft().y + 25.f;
        break;
    case LogSize::SMALL:
        radius = { 15.f, 3.f };
        initialY = game.arena->water.getTopLeft().y + 35.f;
        break;
    }

    initialCenter = { game.arena->playArea.x1 - radius.x, initialY };
    sprite = BoundedRect::create(initialCenter, radius);
    sprite.getBoundingBox().radius.x -= Frog::collisionRadius.x * 0.5f;
    velocity = glm::vec2(speed, 0);

    color = Color(0.627f, 0.322f, 0.176f);
}
