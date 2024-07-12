#pragma once
#include "cinder/Color.h"
#include "cinder/Cinder.h"
#include "cinder/gl/gl.h"

#include "gameState.h"
#include "textBox.h"

enum class BrickColor;

class Scoreboard
{
public:
    Scoreboard();

    int getScore() const { return score; }
    int getLives() const { return livesRemaining; }

    void incrementScore(BrickColor color, int level);
    void incrementScore(int points);
    void incrementLives();
    void decrementLives();
    
    void update();
    void reset(int newHighScore);
    void draw();

private:
    float fontSize = 5.f;
    ci::Color color = ci::Color(0.5f, 0.5f, 0.5f);

    glm::vec2 livesPos{ 10, 2 };
    glm::vec2 highScorePos{ 85, 2 };
    glm::vec2 scorePos{ 40, 2 };

    ::TextBox livesText{ fontSize, livesPos };
    ::TextBox highScoreText{ fontSize, highScorePos };
    ::TextBox scoreText{ fontSize, scorePos };

    int highScore = 0;
    int score = 0;
    int livesRemaining = 3;

    void updateText();
};
