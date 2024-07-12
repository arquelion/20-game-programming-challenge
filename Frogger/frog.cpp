#include "precomp.h"

#include "frog.h"

#include "gameState.h"
#include "view.h"

using namespace ci;
using namespace ci::app;

const glm::vec2 Frog::collisionRadius{ 3.5f, 3.5f };

Frog::Frog()
{
    addAssetDirectory(".");
    auto img = loadImage(loadAsset("frog.png"));
    texture = gl::Texture2d::create(img);
    reset();
}

bool Frog::isDead()
{
    Hit hit;
    for (auto& wall : game.arena->walls)
    {
        auto hit = wall.getBoundingBox().intersectAABB(sprite.getBoundingBox());
        if (hit) return true;
    }

    for (auto& car : game.arena->cars)
    {
        auto hit = car->sprite.getBoundingBox().intersectAABB(sprite.getBoundingBox());
        if (hit) return true;
    }

    // Snake heads

    // Crocodile heads

    // Water
    if (game.arena->water.getBoundingBox().intersectAABB(sprite.getBoundingBox()))
    {
        for (auto& object : game.arena->waterObjects)
        {
            if (object->isCollideable && object->sprite.getBoundingBox().intersectPoint(sprite.getCenter()))
            {
                nextDir = object->velocity;
                return false;
            }
        }
        return true;
    }

    return false;
}

bool Frog::isInHouse() const
{
    Hit hit;
    for (auto& house : game.arena->houses)
    {
        auto hit = house.sprite.getBoundingBox().intersectPoint(sprite.getCenter());
        if (hit && !house.isOccupied)
        {
            house.isOccupied = true;
            return true;
        }
    }
    return false;
}

void Frog::animate()
{
    // TODO: The frog is already round and therefore it should just roll everywhere
}

void Frog::move(Direction dir)
{
    nextDir = glm::vec2(0.f);
    switch (dir)
    {
    case Direction::UP:
        nextDir.y = -10.f;
        break;
    case Direction::DOWN:
        nextDir.y = 10.f;
        break;
    case Direction::LEFT:
        nextDir.x = -10.f;
        break;
    case Direction::RIGHT:
        nextDir.x = 10.f;
        break;
    }
}

void Frog::reset()
{
    sprite = BoundedRect::create(initialCenter, initialRadius);
    sprite.getBoundingBox().radius = collisionRadius;

    game.frogTimeLimit = game.clock.now() + game.timePerFrog;
    farthestYReached = initialCenter.y;
}

void Frog::update()
{
    sprite.translate(nextDir);
    nextDir = glm::vec2(0.f);

    auto currentY = sprite.getCenter().y;
    reachedNewRow = currentY < farthestYReached && !isEqual(currentY, farthestYReached);
    if (reachedNewRow)
        farthestYReached = currentY;
}

void Frog::draw() const
{
    //ci::Color color{ 0.8f, 0.8f, 0.1f };
    //gl::color(color);
    //gl::drawSolidRect(view.getRect(sprite.getTopLeft(), sprite.getBotRight()));
    gl::color(Color::white());
    gl::draw(texture, view.getRect(sprite.getTopLeft(), sprite.getBotRight()));
}
