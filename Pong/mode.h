#pragma once
#include "cinder/app/App.h"

#include "gameState.h"
#include "main.h"

class Mode
{
public:
    Mode(PongClone* app, GameState& game)
        : app(app)
        , game(game)
        , lastUpdateTime(std::chrono::steady_clock::now()) {}

    virtual void mouseMove(ci::app::MouseEvent event) {}
    virtual void keyDown(ci::app::KeyEvent event) {}
    virtual void update() {}
    virtual void draw() {}

protected:
    PongClone* app;
    GameState& game;
    std::chrono::steady_clock::time_point lastUpdateTime;
};
