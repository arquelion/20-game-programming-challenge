#include "scoreboard.h"
#include "utils.h"
#include "view.h"

using namespace ci;

Scoreboard::Scoreboard()
{
    reset(highScore);
}

void Scoreboard::incrementScore(BrickColor color, int level)
{
    switch (color)
    {
    case BrickColor::WHITE:
        score += 50;
        break;
    case BrickColor::ORANGE:
        score += 60;
        break;
    case BrickColor::CYAN:
        score += 70;
        break;
    case BrickColor::GREEN:
        score += 80;
        break;
    case BrickColor::RED:
        score += 90;
        break;
    case BrickColor::BLUE:
        score += 100;
        break;
    case BrickColor::MAGENTA:
        score += 110;
        break;
    case BrickColor::YELLOW:
        score += 120;
        break;
    case BrickColor::SILVER:
        score += 50 * level;
        break;
    default:
        return;
    }

    updateText();
}

void Scoreboard::incrementScore(int points)
{
    score += points;
    updateText();
}

void Scoreboard::incrementLives()
{
    ++livesRemaining;
    updateText();
}

void Scoreboard::decrementLives()
{
    --livesRemaining;
    updateText();
}

void Scoreboard::update()
{
    livesText.update();
    highScoreText.update();
    scoreText.update();
}

void Scoreboard::reset(int newHighScore)
{
    score = 0;
    highScore = newHighScore;
    livesRemaining = 3;
    updateText();
}

void Scoreboard::draw()
{
    livesText.draw();
    highScoreText.draw();
    scoreText.draw();
}

void Scoreboard::updateText()
{
    char buf[128];
    sprintf_s(buf, "Lives: %d", livesRemaining);
    livesText.setText(buf);

    sprintf_s(buf, "High Score: %d", highScore);
    highScoreText.setText(buf);

    sprintf_s(buf, "Score: %d", score);
    scoreText.setText(buf);
}
