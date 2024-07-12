#pragma once
#include "cinder/app/App.h"

#include "mode.h"

class IdleMode : public Mode
{
public:
    IdleMode(PongClone* app, GameState& game) : Mode(app, game)
    {
        game.init();
        game.newGame();
        game.isActive = false;
    }

    void keyDown(ci::app::KeyEvent event) override
    {
        app->next = PongClone::GameMode::PLAY;
    }

    void update() override
    {
        auto now = std::chrono::steady_clock::now();
        auto delta = std::chrono::duration_cast<std::chrono::milliseconds>(now - lastUpdateTime);
        game.update(delta.count() / 1000.f);
        lastUpdateTime = now;
    }

    void draw() override
    {
        game.draw();
    }
};

