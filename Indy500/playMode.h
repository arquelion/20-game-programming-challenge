#pragma once

#include "mode.h"
#include "textBox.h"
#include "utils.h"
#include "view.h"

class PlayMode : public Mode
{
public:
    PlayMode(IndyClone* app)
        : Mode(app)
        , msg(5.f, { 50, 50 })
    {
        game.init();
        game.newGame();
        msg.setText("PAUSED");
    }

    void keyDown(ci::app::KeyEvent event) override
    {
        // TODO
        switch (event.getCode())
        {
        case ci::app::KeyEvent::KEY_p:
            togglePause();
            break;
        case ci::app::KeyEvent::KEY_UP:
            game.accelerate(1);
            break;
        case ci::app::KeyEvent::KEY_DOWN:
            game.accelerate(-1);
            break;
        case ci::app::KeyEvent::KEY_LEFT:
            game.rotate(-1);
            break;
        case ci::app::KeyEvent::KEY_RIGHT:
            game.rotate(1);
            break;
        }
    }

    void update() override
    {
        auto now = game.clock_.current();
        auto delta = std::chrono::duration_cast<std::chrono::milliseconds>(now - lastUpdateTime);
        game.update(delta.count() / 1000.f);
        if (game.isGameOver())
        {
            app->next = IndyClone::GameMode::END;
        }
        lastUpdateTime = now;

        if (isPaused)
        {
            msg.update();
        }
    }

    void draw() const override
    {
        game.draw();

        if (isPaused)
        {
            msg.draw();
        }
    }

private:
    bool isPaused = false;
    ::TextBox msg;

    void togglePause()
    {
        if (game.isActive_ && !game.isGameOver())
        {
            isPaused = !isPaused;

            if (isPaused)
            {
                game.clock_.pause();
            }
            else
            {
                game.clock_.resume();
            }
        }
    }
};
