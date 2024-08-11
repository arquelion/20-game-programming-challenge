#pragma once
#include "cinder/app/App.h"

#include "gameState.h"
#include "main.h"

class BreakoutClone;
class GameState;

class Mode
{
public:
    Mode(BreakoutClone* app, GameState& game)
        : app(app)
        , game(game)
        , lastUpdateTime(game.clock.current()) {}

    virtual void mouseDown(ci::app::MouseEvent event) {}
    virtual void mouseMove(ci::app::MouseEvent event) {}
    virtual void keyDown(ci::app::KeyEvent event) {}
    virtual void update() {}
    virtual void draw() {}

protected:
    BreakoutClone* app;
    GameState& game;
    Clock::time_point lastUpdateTime;
};
