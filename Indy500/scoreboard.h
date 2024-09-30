#pragma once
class Scoreboard;
enum class ScoreSource;

#include "textBox.h"

#include "boost/asio.hpp"

class Scoreboard
{
public:
    Scoreboard();

    int getScore() const { return score; }
    void incrementScore();

    void update(std::chrono::milliseconds lapTime, int highestLap);
    void reset();
    void draw() const;

private:
    float fontSize = 10.f;
    ci::Color color = ci::Color(0.5f, 0.5f, 0.5f);

    glm::vec2 scorePos{ -50, -65 };
    glm::vec2 timerPos{ 30, -65 };

    ::TextBox scoreText{ fontSize, scorePos };
    ::TextBox timerText{ fontSize, timerPos };

    int score = 0;
    int lap = 0;

    void updateText();
};
