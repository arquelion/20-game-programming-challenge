#pragma once
#include <chrono>

#include "cinder/app/App.h"
#include "cinder/gl/gl.h"

#include "mode.h"
#include "view.h"
#include "utils.h"

class PlayMode : public Mode
{
public:
    PlayMode(PongClone* app, GameState& game) : Mode(app, game)
    {
        game.init();
        game.newGame();
    }

    void mouseMove(ci::app::MouseEvent event) override
    {
        glm::vec2 pos{ event.getX(), event.getY() };
        auto time = std::chrono::steady_clock::now();
        game.p1->move(pos, time);
    }

    void keyDown(ci::app::KeyEvent event) override
    {
        using namespace ci::app;

        if (event.getCode() == KeyEvent::KEY_w)
        {
            game.movePaddle(Player::ONE, Direction::UP);
        }
        else if (event.getCode() == KeyEvent::KEY_s)
        {
            game.movePaddle(Player::ONE, Direction::DOWN);
        }
        else if (event.getCode() == KeyEvent::KEY_UP)
        {
            game.movePaddle(Player::TWO, Direction::UP);
        }
        else if (event.getCode() == KeyEvent::KEY_DOWN)
        {
            game.movePaddle(Player::TWO, Direction::DOWN);
        }
    }

    void update() override
    {
        auto now = std::chrono::steady_clock::now();
        auto delta = std::chrono::duration_cast<std::chrono::milliseconds>(now - lastUpdateTime);
        game.update(delta.count() / 1000.f);
        if (game.isGameOver())
        {
            app->next = PongClone::GameMode::END;
        }
        lastUpdateTime = now;
    }

    void draw() override
    {
        game.draw();
    }
};
