#pragma once

#include "mode.h"
#include "textBox.h"
#include "utils.h"
#include "view.h"

class PlayMode : public Mode
{
public:
    PlayMode(FroggerClone* app)
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
            game.frog->move(Direction::UP);
            break;
        case ci::app::KeyEvent::KEY_DOWN:
            game.frog->move(Direction::DOWN);
            break;
        case ci::app::KeyEvent::KEY_LEFT:
            game.frog->move(Direction::LEFT);
            break;
        case ci::app::KeyEvent::KEY_RIGHT:
            game.frog->move(Direction::RIGHT);
            break;
        }
    }

    void update() override
    {
        auto now = game.clock.current();
        auto delta = std::chrono::duration_cast<std::chrono::milliseconds>(now - lastUpdateTime);
        game.update(delta.count() / 1000.f);
        if (game.isGameOver())
        {
            app->next = FroggerClone::GameMode::END;
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
        if (game.isActive && !game.isGameOver())
        {
            isPaused = !isPaused;

            if (isPaused)
            {
                game.clock.pause();
            }
            else
            {
                game.clock.resume();
            }
        }
    }
};
