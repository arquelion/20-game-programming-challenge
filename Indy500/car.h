#pragma once
class Car;

#include "intersection.h"

struct CarData
{
    CarData()
    {
        sprite = BoundedRect::create({ 0, 0 }, { 6.4f, 4.f });
    }

    BoundedRect sprite;

    glm::vec2 velocity;
    Radians heading = (float)M_PI / 4.f;
    float maxAccel = 10.f;
    Radians maxRotation = (float)M_PI / 4.f;
};

class Car
{
public:
    Car();

    void update(float deltaSec);
    void draw() const;

private:
    friend class GameState;
    CarData data;

    ci::Color color;
    ci::gl::Texture2dRef texture;
};
