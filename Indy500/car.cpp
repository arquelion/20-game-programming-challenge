#include "precomp.h"

#include "car.h"

#include "gameState.h"
#include "view.h"

using namespace ci;
using namespace std::chrono;

Car::Car()
    : object({ 60, 50 }, { 6.4f, 4.f })
{
    object.renderObj.velocity = glm::vec2(5, 0);
}

Car& Car::operator=(const NetCar& source)
{
    CarData::operator=(source);
    object = source.object;
    return *this;
}

void Car::loadTexture()
{
    Surface bitmap(loadImage("vehicles.png"));

    Area area = Area(Rectf({ 0, 5 }, { 16, 15 }));
    Surface car{ 16, 10, true };
    object.renderObj.color = Color(0.3f, 0.3f, 1.0f);

    //car.copyFrom(bitmap, area, glm::ivec2(0, -5));
    //ip::flipHorizontal(&car);
    //object.renderObj.texture = gl::Texture2d::create(car);
}

void Car::updateProgress(int marker, int totalMarkers)
{
    if ((currentMarker + 1) % totalMarkers == marker)
    {
        ++currentMarker;
    }
    else if ((currentMarker - 1) % totalMarkers == marker)
    {
        --currentMarker;
        if (currentMarker <= -totalMarkers)
        {
            currentMarker = 0;
        }
    }
    
    if (currentMarker / totalMarkers > highestLap)
    {
        highestLap = std::max(highestLap, int(currentMarker / totalMarkers));
        lapStartTime += lapDuration;
        lapDuration = lapDuration.zero();
    }
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
