#pragma once
#include "cinder/Cinder.h"
#include "cinder/gl/gl.h"

class View
{
public:
    View(float width, float height);

    glm::vec2 getPixelToDesignScale() const;

    float calcScaledValue(float value);

    float calcScreenX(float scalar);
    float calcScreenY(float scalar);
    glm::vec2 calcScaledVec2(glm::vec2 vec);

    std::vector<ci::Rectf> getOffsetRects();

    ci::Rectf getRect(glm::vec2 topLeft, glm::vec2 botRight);

    void updateWindow(float width, float height);

private:
    glm::vec2 view;
    glm::vec2 window;
    
    glm::vec2 scale;
    glm::vec2 offset;
};

extern View view;
