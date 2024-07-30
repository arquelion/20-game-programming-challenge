#pragma once
class Scoreboard;
enum class ScoreSource;

#include "game.h"
#include "textBox.h"

#include "boost/asio.hpp"

class Scoreboard
{
public:
    Scoreboard();

    int getScore() const { return score; }
    void incrementScore();

    void update();
    void reset();
    void draw() const;

private:
    float fontSize = 10.f;
    ci::Color color = ci::Color(0.5f, 0.5f, 0.5f);

    glm::vec2 scorePos{ 60, 3 };
    glm::vec2 timerPos{ 120, 153 };
    ci::Rectf timerBar{ {20, 155}, {110, 160} };

    ::TextBox scoreText{ fontSize, scorePos };
    ::TextBox timerText{ fontSize, timerPos };

    int score = 0;

    void updateText();

    boost::asio::io_context context;
    boost::asio::ip::tcp::resolver aaaaa;
};
