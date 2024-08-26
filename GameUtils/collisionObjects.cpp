#include "collisionObjects.h"

Collideable2D::Collideable2D(glm::vec2 center, glm::vec2 radius)
    : boundingBox(center, radius)
    , renderObj(center, radius)
{
}

auto Collideable2D::operator=(const NetCollideable2D source) -> Collideable2D
{
    boundingBox = source.boundingBox;
    renderObj = source.renderObj;
    return *this;
}

void Collideable2D::translate(glm::vec2 dir)
{
    renderObj.translate(dir);
    boundingBox.translate(dir);
}

void Collideable2D::rotate(Radians delta)
{
    renderObj.rotate(delta);
    boundingBox.rotate(delta);
}

bool Collideable2D::isOffscreen() const
{
    return false;
}

void Collideable2D::reset()
{
    renderObj.reset();
    boundingBox.center = renderObj.sprite.getCenter();
}

void Collideable2D::update(float deltaSec)
{
    renderObj.update(deltaSec);
    boundingBox.center = renderObj.sprite.getCenter();
}

void Collideable2D::draw() const
{
    renderObj.draw();
}


auto NetCollideable2D::operator=(const Collideable2D source) -> NetCollideable2D
{
    boundingBox = source.boundingBox;
    renderObj = source.renderObj;
    return *this;
}


BoundedRect BoundedRect::create(glm::vec2 center, glm::vec2 radius)
{
    return BoundedRect(center, radius);
}

BoundedRect BoundedRect::createFromCorners(glm::vec2 topLeft, glm::vec2 botRight)
{
    glm::vec2 center = (topLeft + botRight) / 2.f;
    glm::vec2 radius = center - topLeft;
    return BoundedRect(center, radius);
}

void BoundedRect::translate(glm::vec2 dir)
{
    center += dir;
    boundingBox.center += dir;
}

// TODOs:
// combine OBB with Renderable2D in Collideable2D
// implement rotation of bounding boxes
// when to block rotation if rotation would result in bounding box collisions? offset?
