#include <vector>

#include "utils.h"
#include "view.h"

using namespace ci;

View::View(float width, float height)
    : view(width, height)
{
    updateWindow(640.f, 480.f);
}

glm::vec2 View::getPixelToDesignScale() const
{
    glm::vec2 unit{ 1.f, 1.f };
    return unit / view / scale;
}

float View::calcScaledValue(float value)
{
    return value * view.y * scale.y;
}

float View::calcScreenX(float scalar)
{
    return scalar * view.x * scale.x + offset.x;
}

float View::calcScreenY(float scalar)
{
    return scalar * view.y * scale.y + offset.y;
}

glm::vec2 View::calcScaledVec2(glm::vec2 vec)
{
    return glm::vec2(
        vec.x * view.x * scale.x + offset.x,
        vec.y * view.y * scale.y + offset.y
    );
}

std::vector<Rectf> View::getOffsetRects()
{
    std::vector<Rectf> rects;
    if (offset.x != 0)
    {
        rects.push_back(Rectf(0.f, 0.f, offset.x, window.y));
        rects.push_back(Rectf(window.x - offset.x, 0.f, window.x, window.y));
    }
    else
    {
        rects.push_back(Rectf(0.f, 0.f, window.x, offset.y));
        rects.push_back(Rectf(0.f, window.y - offset.y, window.x, window.y));
    }
    return rects;
}

ci::Rectf View::getRect(glm::vec2 topLeft, glm::vec2 botRight)
{
    return Rectf(
        topLeft.x * view.x * scale.x + offset.x,
        topLeft.y * view.y * scale.y + offset.y,
        botRight.x * view.x * scale.x + offset.x,
        botRight.y * view.y * scale.y + offset.y
    );
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
}
