#include "precomp.h"

#include "car.h"

#include "view.h"

using namespace ci;

Car::Car()
    : object({ 20, 20 }, { 6.4f, 4.f })
{
    object.renderObj.velocity = glm::vec2(5, 0);
}

Car& Car::operator=(const NetCar& source)
{
    object = source.object;
    return *this;
}

void Car::loadTexture()
{
    Surface bitmap(loadImage("vehicles.png"));

    Area area = Area(Rectf({ 0, 5 }, { 16, 15 }));
    Surface car{ 16, 10, true };
    object.renderObj.color = Color(0.3f, 0.3f, 1.0f);

    car.copyFrom(bitmap, area, glm::ivec2(0, -5));
    ip::flipHorizontal(&car);
    object.renderObj.texture = gl::Texture2d::create(car);
}

void Car::update(float deltaSec)
{
    object.update(deltaSec);
}

void Car::draw() const
{
    object.draw();
}


auto NetCar::operator=(const Car& source) -> NetCar&
{
    CarData::operator=(source);
    object = source.object;
    return *this;
}
