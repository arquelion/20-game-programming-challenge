#pragma once
struct CarData;
class Car;
struct NetCar;

#include "collisionObjects.h"

struct CarData
{
    float maxAccel = 10.f;
    Radians maxRotation = (float)M_PI / 4.f;
};

class Car : public CarData
{
public:
    Car();
    Car& operator=(const NetCar& source);

    void loadTexture();

    void update(float deltaSec);
    void draw() const;

    Collideable2D object;
};

struct NetCar : public CarData
{
    auto operator=(const Car& source) -> NetCar&;

    NetCollideable2D object;
};