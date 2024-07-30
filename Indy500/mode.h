#pragma once
class Mode;

#include "gameState.h"
#include "main.h"

class Mode
{
public:
    Mode(IndyClone* app)
        : app(app)
        , lastUpdateTime(game.clock.now()) {}

    virtual void keyDown(ci::app::KeyEvent event) {}
    virtual void update() {}
    virtual void draw() const {}

protected:
    IndyClone* app;
    Clock::time_point lastUpdateTime;
};
