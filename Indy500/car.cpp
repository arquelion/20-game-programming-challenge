#include "precomp.h"

#include "car.h"

#include "view.h"

using namespace ci;

Car::Car()
{
    Surface bitmap(loadImage("vehicles.png"));

    Area area = Area(Rectf({ 0, 5 }, { 16, 15 }));
    Surface car{ 16, 10, true };
    glm::vec2 radius{ 6.4f, 4.f };
    color = Color(0.3f, 0.3f, 1.0f);

    car.copyFrom(bitmap, area, glm::ivec2(0, -5));
    ip::flipHorizontal(&car);
    texture = gl::Texture2d::create(car);

    sprite = BoundedRect::create({ 20, 20 }, radius);
    velocity = glm::vec2(5, 0);
}

void Car::update(float deltaSec)
{
    sprite.getCenter() += velocity * deltaSec;
}

void Car::draw() const
{
    gl::pushModelMatrix();
    gl::translate(sprite.getCenter());
    gl::rotate(heading);
    gl::color(Color::white());
    gl::draw(texture, Rectf(sprite.getTopLeft() - sprite.getCenter(), sprite.getBotRight() - sprite.getCenter()));
    gl::popModelMatrix();
}
