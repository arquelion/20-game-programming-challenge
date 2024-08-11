#pragma once
#include <chrono>

#include "cinder/app/App.h"
#include "cinder/gl/gl.h"

#include "mode.h"
#include "textBox.h"
#include "utils.h"
#include "view.h"

class PlayMode : public Mode
{
public:
    PlayMode(BreakoutClone* app, GameState& game)
        : Mode(app, game)
        , msg(5.f, { 50, 50 })
    {
        game.init();
        game.newGame();
        game.arena->loadLevel(game.currentLevel);
        msg.setText("PAUSED");
    }

    void mouseDown(ci::app::MouseEvent event) override
    {
        if (!isPaused)
        {
            game.balls[0]->release();
            game.paddle->fireLaser();
        }
    }

    void mouseMove(ci::app::MouseEvent event) override
    {
        if (!isPaused)
        {
            glm::vec2 pos{ event.getX(), event.getY() };
            game.paddle->move(pos * view.getPixelToDesignScale());
        }
    }

    void keyDown(ci::app::KeyEvent event) override
    {
        if (event.getCode() == ci::app::KeyEvent::KEY_p) {
            togglePause();
        }
    }
     
    void update() override
    {
        auto now = game.clock.current();
        auto delta = std::chrono::duration_cast<std::chrono::milliseconds>(now - lastUpdateTime);
        game.update(delta.count() / 1000.f);
        if (game.isGameOver())
        {
            app->next = BreakoutClone::GameMode::END;
        }
        lastUpdateTime = now;

        if (isPaused)
        {
            msg.update();
        }
    }

    void draw() override
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
