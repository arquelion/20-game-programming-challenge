#pragma once
class Car;

#include "object2D.h"

enum class CarType : uint8_t
{
    TRUCK,
    RACER_1,
    CAR,
    TRACTOR,
    RACER_2,
};

class Car : public Object2D
{
public:
    Car(CarType type, float speed);

private:
    CarType type;
};
