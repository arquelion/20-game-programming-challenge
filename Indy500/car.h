#pragma once
class Car;

#include "intersection.h"

class Car
{
public:
    Car();

    void update(float deltaSec);
    void draw() const;

private:
    BoundedRect sprite;
    glm::vec2 velocity;
    Radians heading = (float)M_PI / 4.f;

    ci::Color color;
    ci::gl::Texture2dRef texture;
};
