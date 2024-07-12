#pragma once
class Scoreboard;
enum class ScoreSource;

#include "game.h"
#include "textBox.h"

enum class ScoreSource
{
    HOUSE,
    FLY,
    LADY_FROG,
    MOVEMENT,
};

class Scoreboard
{
public:
    Scoreboard();

    int getScore() const { return score; }

    void incrementScore(ScoreSource source);

    void update();
    void reset(int newHighScore);
    void draw() const;

private:
    float fontSize = 10.f;
    ci::Color color = ci::Color(0.5f, 0.5f, 0.5f);

    glm::vec2 livesPos{ 15, 3 };
    glm::vec2 scorePos{ 60, 3 };
    glm::vec2 highScorePos{ 115, 3 };
    glm::vec2 timerPos{ 120, 153 };
    ci::Rectf timerBar{ {20, 155}, {110, 160} };

    ::TextBox livesText{ fontSize, livesPos };
    ::TextBox highScoreText{ fontSize, highScorePos };
    ::TextBox scoreText{ fontSize, scorePos };
    ::TextBox timerText{ fontSize, timerPos };

    int highScore = 0;
    int score = 0;

    void updateText();
};
