#pragma once
class Arena;

#include "car.h"

#include "game.h"
#include "intersection.h"

struct ArenaData
{
    ci::Rectf playArea{ {-82.5, -70}, {82.5, 70} };
    std::array<AABB, 4> walls{ {
        {glm::vec2(0, -75), glm::vec2(82.5, 5)},
        {glm::vec2(0, 75), glm::vec2(82.5, 5)},
        {glm::vec2(-87.5, 0), glm::vec2(5, 70)},
        {glm::vec2(87.5, 0), glm::vec2(5, 70)}
    } };
};

class Arena
{
public:
    Arena();

    void loadLevel(int level);

    void update(float deltaSec);
    void draw() const;

    ArenaData data;

private:
    friend class GameState;
};
