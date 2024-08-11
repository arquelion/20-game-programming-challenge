#pragma once
#include "mode.h"
#include "textBox.h"

class IdleMode : public Mode
{
public:
    IdleMode(FroggerClone* app)
        : Mode(app)
        , msg(10.f, glm::vec2(50, 30))
    {
        game.init();
        game.newGame();
        game.isActive = false;

        msg.setText("CLICK TO START");
        msg.setColor(ci::Color(0.2f, 0.2f, 1.0f));
    }

    void keyDown(ci::app::KeyEvent event) override
    {
        app->next = FroggerClone::GameMode::PLAY;
    }

    void update() override
    {
        auto now = game.clock.current();
        auto delta = std::chrono::duration_cast<std::chrono::milliseconds>(now - lastUpdateTime);
        game.update(delta.count() / 1000.f);
        msg.update();
        lastUpdateTime = now;
    }

    void draw() const override
    {
        game.draw();
        msg.draw();
    }

private:
    ::TextBox msg;
};

