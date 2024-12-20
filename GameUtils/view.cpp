#include <vector>

#include "view.h"
#include "../GameUtils/utils.h"

using namespace ci;

View::View(float width, float height)
    : view(width, height)
{
    //updateWindow(640.f, 480.f);
}

glm::vec2 View::getPixelToDesignScale() const
{
    glm::vec2 unit{ 1.f, 1.f };
    return unit / scale;
}

float View::calcScaledValue(float value)
{
    return value * scale.y;
}

float View::calcScreenX(float scalar)
{
    return (scalar + offsetToOrigin.x) * scale.x + offset.x;
}

float View::calcScreenY(float scalar)
{
    return (scalar + offsetToOrigin.y) * scale.y + offset.y;
}

glm::vec2 View::getScreenVec2(glm::vec2 vec)
{
    return (vec + offsetToOrigin) * scale + offset;
}

void View::setOrigin(Origin loc)
{
    origin = loc;
    if (loc == Origin::UPPER_LEFT)
    {
        offsetToOrigin = glm::vec2(0);
    }
    else
    {
        offsetToOrigin = view / 2.f;
    }
}

Rectf View::getRect(glm::vec2 topLeft, glm::vec2 botRight)
{
    return Rectf(
        (topLeft + offsetToOrigin) * scale + offset,
        (botRight + offsetToOrigin) * scale + offset
    );
}

Rectf View::getRect(Rectf designRect)
{
    return getRect(designRect.getUpperLeft(), designRect.getLowerRight());
}

void View::updateWindow(float width, float height)
{
    window = glm::vec2(width, height);
    scale = glm::vec2(window.x / view.x, window.y / view.y);
    if (!isEqual(scale.x, scale.y))
    {
        int index = 0;
        if (scale.x > scale.y)
        {
            index = 0;
            scale.x = scale.y;
            offset.y = 0.f;
        }
        else
        {
            index = 1;
            scale.y = scale.x;
            offset.x = 0.f;
        }
        offset[index] = 0.5f * (window[index] - (view[index] * scale[index]));
    }
    else
    {
        offset = { 0.f, 0.f };
    }

    auto clientDesignSpace = window / scale;
    gl::setMatricesWindow(clientDesignSpace);
    if (origin == Origin::CENTER)
    {
        gl::translate(clientDesignSpace / 2.f);
    }
}
