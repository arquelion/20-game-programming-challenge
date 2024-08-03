#pragma once
class Arena;

#include "car.h"

#include "game.h"
#include "intersection.h"

class Arena
{
public:
    Arena();

    void loadLevel(int level);

    void update(float deltaSec);
    void draw() const;

    ci::Rectf playArea{ };

private:
    friend class GameState;
};
