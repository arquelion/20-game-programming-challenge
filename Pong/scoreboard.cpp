#include "scoreboard.h"
#include "utils.h"
#include "view.h"

using namespace ci;

Scoreboard::Scoreboard(glm::vec2 pos)
    : scorePos(pos)
{
    scoreFont = Font("Calibri", view.calcScaledValue(fontSizeNorm));
}

void Scoreboard::incrementScore(int player)
{
    if (player == 1) p1Score++;
    else if (player == 2) p2Score++;
    else
    {
        /* should not get here */
    }
}

void Scoreboard::setFontSize()
{
    auto size = view.calcScaledValue(fontSizeNorm);
    if (!isEqual(size, scoreFont.getSize()))
    {
        scoreFont = Font(scoreFont.getName(), size);
    }
}

void Scoreboard::reset()
{
    p1Score = 0;
    p2Score = 0;
}

void Scoreboard::draw()
{
    char score[128];
    sprintf_s(score, "%d : %d", p1Score, p2Score);
    gl::drawStringCentered(score, view.getViewVec2(scorePos), color, scoreFont);
}
