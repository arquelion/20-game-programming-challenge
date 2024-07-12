#pragma once
#include <chrono>

#include "cinder/app/App.h"

#include "mode.h"
#include "view.h"

class EndMode : public Mode
{
public:
    EndMode(PongClone* app, GameState& game)
        : Mode(app, game)
        , start(std::chrono::steady_clock::now()) {}

    void update() override
    {
        using namespace std::chrono_literals;
        if (std::chrono::steady_clock::now() - start >= 5s)
        {
            app->next = PongClone::GameMode::IDLE;
        }
    }

    void draw() override
    {
        auto scores = game.scoreboard->getScores();
        char endMsg[128];
        sprintf_s(endMsg, "Player %d wins!", ((game.winner == Player::ONE) ? 1 : 2));
        ci::gl::drawStringCentered(
            endMsg,
            view.calcScaledVec2({ 0.5f, 0.3f }),
            ci::Color(0.1f, 1.0f, 0.1f),
            ci::Font("Calibri", view.calcScaledValue(0.1f))
        );
        game.showWinner = false;
    }

private:
    std::chrono::steady_clock::time_point start;
};
