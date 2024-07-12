#pragma once
class Object2D;

#include "intersection.h"

class Object2D
{
public:
    Object2D() {}

    virtual void update();
    virtual void draw() const;

    BoundedRect sprite;
    glm::vec2 velocity;
    bool isCollideable = true;

protected:
    ci::gl::Texture2dRef texture;
    ci::Color color;

    glm::vec2 initialCenter;

    bool isOffscreen() const;
    void reset();
};
