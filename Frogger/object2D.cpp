#include "precomp.h"

#include "object2D.h"
#include "gameState.h"

using namespace ci;

void Object2D::update(float deltaSec)
{
    if (isOffscreen())
    {
        reset();
    }
    sprite.translate(velocity * deltaSec * 60.f);
}

void Object2D::draw() const
{
    if (!texture)
    {
        gl::color(color);
        gl::drawSolidRect(view.getRect(sprite.getTopLeft(), sprite.getBotRight()));
    }
    else
    {
        gl::color(Color::white());
        gl::draw(texture, view.getRect(sprite.getTopLeft(), sprite.getBotRight()));
    }
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
