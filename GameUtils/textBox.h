#pragma once
#include "cinder/Color.h"
#include "cinder/Cinder.h"
#include "cinder/gl/gl.h"

#include "utils.h"
#include "view.h"

class TextBox
{
public:
    TextBox() {}

    TextBox(float size, glm::vec2 center)
        : size(size)
        , center(center)
    {
        font = ci::Font("Calibri", view.calcScaledValue(size));
    }

    void setFontToScale()
    {
        auto newSize = view.calcScaledValue(size);
        if (!isEqual(newSize, font.getSize()))
        {
            font = ci::Font("Calibri", view.calcScaledValue(size));
        }
    }

    void reset(float size, glm::vec2 center)
    {
        this->size = size;
        this->center = center;
        font = ci::Font("Calibri", view.calcScaledValue(size));
    }

    TextBox& setText(std::string newText)
    {
        text = newText;
        return *this;
    }

    TextBox& setColor(ci::Color newColor)
    {
        color = newColor;
        return *this;
    }

    void update()
    {
        setFontToScale();
    }

    void draw() const
    {
        ci::gl::drawStringCentered(text, view.getScreenVec2(center), color, font);
    }

private:
    ci::Font font;
    float size = 5.f;
    ci::Color color = ci::Color(0.5f, 0.5f, 0.5f);

    glm::vec2 center;
    std::string text;
};
