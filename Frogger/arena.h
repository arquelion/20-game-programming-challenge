#pragma once
class Arena;

#include "car.h"
#include "house.h"

#include "game.h"
#include "intersection.h"

class Arena
{
public:
    Arena();

    void loadLevel(int level);

    void update();
    void draw() const;

    ci::Rectf playArea{ {0, 15}, {140, 150} };
    BoundedRect water;

private:
    friend class Frog;
    friend class GameState;
    friend class House;

    std::vector<std::shared_ptr<Car>> cars;
    std::vector<std::shared_ptr<Object2D>> waterObjects;

    std::vector<BoundedRect> grasses;
    std::vector<House> houses;
    std::vector<BoundedRect> walls;

    static const ci::Color grassColor;
    static const ci::Color waterColor;
};
