#include "precomp.h"

#include "gameState.h"
#include "scoreboard.h"

using namespace ci;

Scoreboard::Scoreboard()
{
    reset(highScore);
}

void Scoreboard::incrementScore(ScoreSource source)
{
    switch (source)
    {
    case ScoreSource::HOUSE:
        score += 50; // Base house points
        score += 10 * (int)(duration_cast<std::chrono::milliseconds>(game.frogTimeLimit - game.clock.now()).count() / 500); // Bonus points for time
        break;
    case ScoreSource::FLY:
    case ScoreSource::LADY_FROG:
        score += 200;
        break;
    case ScoreSource::MOVEMENT:
        score += 10;
        break;
    default:
        throw std::logic_error("non-existent score source");
    }

    if (game.frogsRemaining <= 0)
    {
        score += 1000;
    }
    if (!game.extraLifeGained && score >= 20000)
    {
        ++game.livesRemaining;
        game.extraLifeGained = true;
    }
    updateText();
}

void Scoreboard::update()
{
    livesText.update();
    highScoreText.update();
    scoreText.update();
    timerText.update();
    updateText();
}

void Scoreboard::reset(int newHighScore)
{
    score = 0;
    highScore = newHighScore;
    game.livesRemaining = 7;
    updateText();
}

void Scoreboard::draw() const
{
    livesText.draw();
    highScoreText.draw();
    scoreText.draw();
    timerText.draw();

    auto upperLeft = timerBar.getUpperLeft();
    auto percent = std::clamp((game.frogTimeLimit - game.clock.now()).count() / (float)game.timePerFrog.count(), 0.f, 1.f);
    auto timeRemaining = (timerBar.getLowerRight().x - upperLeft.x) * percent;
    upperLeft.x = timerBar.getLowerRight().x - timeRemaining;

    if (percent <= 0.167f)
    {
        gl::color(Color(0.8f, 0.2f, 0.2f));
    }
    else
    {
        gl::color(Color(0.2f, 0.8f, 0.2f));
    }
    gl::drawSolidRect(view.getRect(upperLeft, timerBar.getLowerRight()));
}

void Scoreboard::updateText()
{
    char buf[128];
    sprintf_s(buf, "Lives: %d", game.livesRemaining);
    livesText.setText(buf);

    sprintf_s(buf, "High Score: %d", highScore);
    highScoreText.setText(buf);

    sprintf_s(buf, "Score: %d", score);
    scoreText.setText(buf);

    timerText.setText("Time");
}
