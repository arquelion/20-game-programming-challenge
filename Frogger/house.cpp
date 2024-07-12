#include "precomp.h"

#include "house.h"

#include "frog.h"

#include "arena.h"
#include "view.h"

using namespace ci;

House::House(BoundedRect rect)
{
    sprite = rect;
    sprite.getBoundingBox().radius -= Frog::collisionRadius;
}

void House::draw() const
{
    if (isOccupied)
    {
        Object2D::draw();
    }
    else
    {
        gl::color(Arena::waterColor);
        gl::drawSolidRect(view.getRect(sprite.getTopLeft(), sprite.getBotRight()));
    }
}
