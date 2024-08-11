#pragma once
#include "cinder/app/App.h"

#include "mode.h"
#include "textBox.h"

class IdleMode : public Mode
{
public:
    IdleMode(BreakoutClone* app, GameState& game)
        : Mode(app, game)
        , msg(10.f, glm::vec2(50, 30))
    {
        game.init();
        game.newGame();
        game.isActive = false;

        msg.setText("CLICK TO START");
        msg.setColor(ci::Color(0.2f, 0.2f, 1.0f));
    }

    void mouseDown(ci::app::MouseEvent event) override
    {
        app->next = BreakoutClone::GameMode::PLAY;
    }

    void keyDown(ci::app::KeyEvent event) override
    {
        app->next = BreakoutClone::GameMode::PLAY;
    }

    void update() override
    {
        auto now = game.clock.current();
        auto delta = std::chrono::duration_cast<std::chrono::milliseconds>(now - lastUpdateTime);
        game.update(delta.count() / 1000.f);
        msg.update();
        lastUpdateTime = now;
    }

    void draw() override
    {
        game.draw();
        msg.draw();
    }

private:
    ::TextBox msg;
};
