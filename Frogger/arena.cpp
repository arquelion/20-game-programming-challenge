#include "precomp.h"

#include "crocodile.h"
#include "log.h"
#include "turtle.h"

#include "arena.h"
#include "gameState.h"

using namespace ci;

const ci::Color Arena::grassColor = ci::Color(0.6f, 0.2f, 0.6f);
const ci::Color Arena::waterColor = ci::Color(0.2f, 0.2f, 0.8f);

Arena::Arena()
{
    // Road
    glm::vec2 topLeft = { 0, 90 };
    glm::vec2 botRight = { 140, 140 };
    road.sprite = BoundedRect::createFromCorners(topLeft, botRight);
    road.color = Color(0.3f, 0.3f, 0.3f);

    // Lanes
    topLeft = { 0, 15 };
    botRight = { 140, 30 };
    grasses.push_back(BoundedRect::createFromCorners(topLeft, botRight));
    walls.push_back(BoundedRect::createFromCorners(topLeft, botRight));

    topLeft = { 0, 80 };
    botRight = { 140, 90 };
    grasses.push_back(BoundedRect::createFromCorners(topLeft, botRight));

    topLeft = { 0, 140 };
    botRight = { 140, 150 };
    grasses.push_back(BoundedRect::createFromCorners(topLeft, botRight));

    // Houses
    glm::vec2 padding{ 4.5, 18 };
    glm::vec2 size{ 15, 12 };
    auto spacing = 14.f;

    for (int i = 0; i < 5; ++i)
    {
        topLeft = padding;
        topLeft.x += (spacing + size.x) * i;
        botRight = topLeft + size;
        houses.push_back(BoundedRect::createFromCorners(topLeft, botRight));
    }

    // Side walls
    topLeft = { playArea.x1 - 16, playArea.y1 };
    botRight = { playArea.x1, playArea.y2 };
    walls.push_back(BoundedRect::createFromCorners(topLeft, botRight));

    topLeft = { playArea.x2, playArea.y1 };
    botRight = { playArea.x2 + 16, playArea.y2 };
    walls.push_back(BoundedRect::createFromCorners(topLeft, botRight));

    topLeft = { playArea.x1, playArea.y2 };
    botRight = { playArea.x2, playArea.y2 + 16 };
    walls.push_back(BoundedRect::createFromCorners(topLeft, botRight));

    // Water
    water = BoundedRect::create(glm::vec2(70, 55), glm::vec2(70, 25));
}

void Arena::loadLevel(int level)
{
    cars.clear();
    waterObjects.clear();

    for (auto& house : houses)
    {
        house.isOccupied = false;
    }

    cars.push_back(std::make_shared<Car>(CarType::TRUCK, -0.5f));
    cars.push_back(std::make_shared<Car>(CarType::RACER_1, 0.7f));
    cars.push_back(std::make_shared<Car>(CarType::CAR, 0.5f));
    cars.push_back(std::make_shared<Car>(CarType::TRACTOR, -0.3f));
    cars.push_back(std::make_shared<Car>(CarType::RACER_2, 0.2f));

    waterObjects.push_back(std::make_shared<Croc>(0.2f));
    //waterObjects.push_back(std::make_shared<Log>(LogSize::MEDIUM, 0.2f));
    waterObjects.push_back(std::make_shared<Log>(LogSize::MEDIUM, 0.2f));
    waterObjects.back()->sprite.translate(glm::vec2(-0.5f * playArea.getWidth(), 0));
    waterObjects.push_back(std::make_shared<Log>(LogSize::LARGE, 0.3f));
    waterObjects.push_back(std::make_shared<Log>(LogSize::SMALL, 0.1f));
    waterObjects.push_back(std::make_shared<Log>(LogSize::SMALL, 0.1f));
    waterObjects.back()->sprite.translate(glm::vec2(-0.33f * playArea.getWidth(), 0));
    waterObjects.push_back(std::make_shared<Log>(LogSize::SMALL, 0.1f));
    waterObjects.back()->sprite.translate(glm::vec2(-0.67f * playArea.getWidth(), 0));

    waterObjects.push_back(std::make_shared<Turtle>(TurtlePod::DOUBLE, -0.3f, std::chrono::milliseconds(500)));
    waterObjects.push_back(std::make_shared<Turtle>(TurtlePod::DOUBLE, -0.3f, std::chrono::milliseconds(0)));
    waterObjects.back()->sprite.translate(glm::vec2(-0.33f * playArea.getWidth(), 0));
    waterObjects.push_back(std::make_shared<Turtle>(TurtlePod::DOUBLE, -0.3f, std::chrono::milliseconds(0)));
    waterObjects.back()->sprite.translate(glm::vec2(-0.67f * playArea.getWidth(), 0));

    waterObjects.push_back(std::make_shared<Turtle>(TurtlePod::TRIPLE, -0.2f, std::chrono::milliseconds(1000)));
    waterObjects.push_back(std::make_shared<Turtle>(TurtlePod::TRIPLE, -0.2f, std::chrono::milliseconds(0)));
    waterObjects.back()->sprite.translate(glm::vec2(-0.4f * playArea.getWidth(), 0));
}

void Arena::update(float deltaSec)
{
    for (auto& car : cars)
    {
        car->update(deltaSec);
    }

    for (auto& object : waterObjects)
    {
        object->update(deltaSec);
    }
}

void Arena::draw() const
{
    road.draw();

    gl::color(grassColor);
    for (auto& grass : grasses)
    {
        gl::drawSolidRect(view.getRect(grass.getTopLeft(), grass.getBotRight()));
    }

    gl::color(waterColor);
    gl::drawSolidRect(view.getRect(water.getTopLeft(), water.getBotRight()));
   
    for (auto& house : houses)
    {
        house.draw();
    }

    for (auto& car : cars)
    {
        car->draw();
    }

    for (auto& object : waterObjects)
    {
        object->draw();
    }
}
