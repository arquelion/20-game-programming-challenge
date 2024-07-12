#pragma once
class Mode;

#include "gameState.h"
#include "main.h"

class Mode
{
public:
    Mode(FroggerClone* app)
        : app(app)
        , lastUpdateTime(game.clock.now()) {}

    virtual void keyDown(ci::app::KeyEvent event) {}
    virtual void update() {}
    virtual void draw() const {}

protected:
    FroggerClone* app;
    Clock::time_point lastUpdateTime;
};
