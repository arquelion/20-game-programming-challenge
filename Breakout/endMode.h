#pragma once
#include "cinder/app/App.h"

#include "clock.h"
#include "mode.h"
#include "textBox.h"
#include "view.h"

class EndMode : public Mode
{
public:
    EndMode(BreakoutClone* app, GameState& game)
        : Mode(app, game)
        , start(game.clock.current())
        , endMsg(10.f, glm::vec2(50, 10))
        , highScoreList(7.5f, glm::vec2(50, 30))
    {
        endMsg.setColor(ci::Color(0.2f, 0.2f, 1.0f));
        highScoreList.setColor(ci::Color(0.2f, 0.2f, 1.0f));

        auto score = game.scoreboard->getScore();
        auto& highScores = game.highScores;

        char endMsgBuf[128];
        sprintf_s(endMsgBuf, "Game Over\nYour score: %d", score);
        endMsg.setText(endMsgBuf);

        for (int i = 0; i < 5; ++i)
        {
            if (score > highScores[i])
            {
                highScores.insert(highScores.begin() + i, score);
                highScores.pop_back();
                break;
            }
        }

        char highScoreListBuf[128];
        sprintf_s(highScoreListBuf, "1. %d\n2. %d\n3. %d\n4. %d\n5. %d", highScores[0], highScores[1], highScores[2], highScores[3], highScores[4]);
        highScoreList.setText(highScoreListBuf);
    }

    void update() override
    {
        using namespace std::chrono_literals;
        if (game.clock.current() - start >= 5s)
        {
            app->next = BreakoutClone::GameMode::IDLE;
        }
    }

    void draw() override
    {
        endMsg.draw();
        highScoreList.draw();
    }

private:
    Clock::time_point start;
    ::TextBox endMsg;
    ::TextBox highScoreList;
};
