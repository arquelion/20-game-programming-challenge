#pragma once
class Track;

#include "car.h"

#include "game.h"
#include "intersection.h"
#include "object2D.h"

struct TrackData
{
    enum class Direction
    {
        CLOCKWISE,
        COUNTERCLOCKWISE,
    } dir;

    std::vector<OBB> lapMarkers;
    struct
    {
        glm::vec2 center = { 0, 0 };
        Radians angle = 0.f;
    } startingLine;

    ci::Rectf playArea{ {-82.5, -70}, {82.5, 70} };
    std::array<OBB, 4> walls{ {
        {glm::vec2(0, -75), glm::vec2(82.5, 5)},
        {glm::vec2(0, 75), glm::vec2(82.5, 5)},
        {glm::vec2(-87.5, 0), glm::vec2(5, 70)},
        {glm::vec2(87.5, 0), glm::vec2(5, 70)}
    } };
    std::vector<OBB> collideables;
};

class Track
{
public:
    Track();

    void loadLevel(int level);

    void update(float deltaSec);
    void draw() const;

    TrackData data;

private:
    friend class GameState;
    std::vector<Object2D> terrain;
};
