#include "precomp.h"

#include "log.h"

#include "gameState.h"

using namespace ci;

Log::Log(LogSize size, float speed)
    : size(size)
{
    Surface bitmap(loadImage("logs.png"));

    Area area;
    Surface log;
    glm::vec2 radius;
    float initialY = 0.f;
    switch (size)
    {
    case LogSize::MEDIUM:
        radius = { 20.268f, 4.f };
        initialY = game.arena->water.getTopLeft().y + 5.f;

        area = Area(Rectf({ 2, 17 }, { 78, 32 }));
        log = Surface(76, 15, true);
        log.copyFrom(bitmap, area, glm::ivec2(-2, -17));
        texture = gl::Texture2d::create(log);
        break;
    case LogSize::LARGE:
        radius = { 28.8f, 4.f };
        initialY = game.arena->water.getTopLeft().y + 25.f;

        area = Area(Rectf({ 2, 33 }, { 110, 48 }));
        log = Surface(108, 15, true);
        log.copyFrom(bitmap, area, glm::ivec2(-2, -33));
        texture = gl::Texture2d::create(log);
        break;
    case LogSize::SMALL:
        radius = { 11.733f, 4.f };
        initialY = game.arena->water.getTopLeft().y + 35.f;

        area = Area(Rectf({ 2, 1 }, { 46, 16 }));
        log = Surface(44, 15, true);
        log.copyFrom(bitmap, area, glm::ivec2(-2, -1));
        texture = gl::Texture2d::create(log);
        break;
    }

    initialCenter = { game.arena->playArea.x1 - radius.x, initialY };
    sprite = BoundedRect::create(initialCenter, radius);
    sprite.getBoundingBox().radius.x -= Frog::collisionRadius.x * 0.5f;
    velocity = glm::vec2(speed, 0);

    color = Color(0.627f, 0.322f, 0.176f);
}


