#include "precomp.h"

#include "object2D.h"
#include "gameState.h"

using namespace ci;

void Object2D::update()
{
    if (isOffscreen())
    {
        reset();
    }
    sprite.translate(velocity);
}

void Object2D::draw() const
{
    gl::color(color);
    gl::drawSolidRect(view.getRect(sprite.getTopLeft(), sprite.getBotRight()));
}

bool Object2D::isOffscreen() const
{
    return (signum(velocity.x) == 1 && sprite.getTopLeft().x > game.arena->playArea.x2)
        || (signum(velocity.x) == -1 && sprite.getBotRight().x < game.arena->playArea.x1);
}

void Object2D::reset()
{
    sprite.setCenter(initialCenter);
}
