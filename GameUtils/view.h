#pragma once
#include "cinder/Cinder.h"
#include "cinder/gl/gl.h"

class View
{
public:
    enum class Origin
    {
        UPPER_LEFT,
        CENTER,
    };

    using DesignScaleVec2 = glm::vec2;
    using PixelScaleVec2 = glm::vec2;

    View(float width, float height);

    glm::vec2 getPixelToDesignScale() const;

    float calcScaledValue(float value);

    float calcScreenX(float scalar);
    float calcScreenY(float scalar);
    glm::vec2 getScreenVec2(glm::vec2 vec);

    void setOrigin(Origin loc);

    ci::Rectf getRect(glm::vec2 topLeft, glm::vec2 botRight);
    ci::Rectf getRect(ci::Rectf designRect);
    glm::vec2 getSize() { return view; }

    void updateWindow(float width, float height);

private:
    DesignScaleVec2 view;
    PixelScaleVec2 window;
    Origin origin;
    DesignScaleVec2 offsetToOrigin;

    glm::vec2 scale; // Design to pixel
    PixelScaleVec2 offset;
};

extern View view;
extern float epsilon;
