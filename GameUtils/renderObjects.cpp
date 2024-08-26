#include "renderObjects.h"

using namespace ci;

Renderable2D::Renderable2D(glm::vec2 center, glm::vec2 radius)
    : sprite(center - radius, center + radius)
    , initialCenter(center)
{
    
}

void Renderable2D::update(float deltaSec)
{
    translate(velocity * deltaSec);
}

void Renderable2D::draw() const
{
    gl::ScopedModelMatrix();
    gl::translate(sprite.getCenter());
    gl::rotate(heading);

    if (!texture)
    {
        gl::color(color);
        gl::drawSolidRect(sprite);
    }
    else
    {
        gl::color(Color::white());
        gl::draw(texture, sprite);
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

// TODOs:
// store rotation/translation matrices, calc transform live
// 
