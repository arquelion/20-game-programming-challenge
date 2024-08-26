#include "renderObjects.h"

using namespace ci;

Renderable2D::Renderable2D()
    : heading(0)
    , initialHeading(0)
{
}

Renderable2D::Renderable2D(glm::vec2 center, glm::vec2 radius)
    : sprite(center - radius, center + radius)
    , heading(0)
    , initialCenter(center)
    , initialHeading(0)
{
}

auto Renderable2D::operator=(const NetRenderable2D& source) -> Renderable2D&
{
    sprite = source.sprite;
    heading = source.heading;
    velocity = source.velocity;
    initialCenter = source.initialCenter;
    initialHeading = source.initialHeading;
    return *this;
}

void Renderable2D::update(float deltaSec)
{
    translate(velocity * deltaSec);
}

void Renderable2D::draw() const
{
    auto tempModelMatrix = gl::ScopedModelMatrix();
    auto center = sprite.getCenter();
    gl::translate(center);
    gl::rotate(heading);

    if (!texture)
    {
        gl::color(color);
        gl::drawSolidRect(sprite - center);
    }
    else
    {
        gl::color(Color::white());
        gl::draw(texture, sprite - center);
    }
}

void Renderable2D::translate(glm::vec2 dir)
{
    sprite += dir;
}

void Renderable2D::rotate(Radians delta)
{
    heading += delta;
}

bool Renderable2D::isOffscreen() const
{
    return false;
}

void Renderable2D::reset()
{
    sprite.offsetCenterTo(initialCenter);
    heading = initialHeading;
    velocity = { 0, 0 };
}


auto NetRenderable2D::operator=(const Renderable2D& source) -> NetRenderable2D&
{
    sprite = source.sprite;
    heading = source.heading;
    velocity = source.velocity;
    initialCenter = source.initialCenter;
    initialHeading = source.initialHeading;
    return *this;
}

// TODOs:
// store rotation/translation matrices, calc transform live
// 
