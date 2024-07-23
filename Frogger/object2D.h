#pragma once
class Object2D;

#include "intersection.h"

class Object2D
{
public:
    Object2D() {}

    virtual void update(float deltaSec);
    virtual void draw() const;

    bool isOffscreen() const;
    void reset();

    BoundedRect sprite;
    glm::vec2 velocity;
    bool isCollideable = true;

    ci::Color color;
    ci::gl::Texture2dRef texture;

    glm::vec2 initialCenter;
};
