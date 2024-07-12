#pragma once
#include "cinder/Color.h"
#include "cinder/Cinder.h"
#include "cinder/gl/gl.h"

class Scoreboard
{
public:
    Scoreboard() {}
    Scoreboard(glm::vec2 pos);

    glm::vec2 getScores() const { return { p1Score, p2Score }; }
    void incrementScore(int player);
    void setFontSize();

    void reset();
    void draw();

private:
    float fontSizeNorm = 0.1f;

    ci::Font scoreFont;
    glm::vec2 scorePos;
    int p1Score = 0;
    int p2Score = 0;

    ci::Color color = ci::Color(0.1f, 1.0f, 0.1f);
};
