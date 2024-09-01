#include "precomp.h"

#include "scoreboard.h"
#include "utils.h"
#include "view.h"

using namespace ci;

Scoreboard::Scoreboard()
{
    reset();
}

void Scoreboard::incrementScore()
{
    // TODO: pellet capture
}

void Scoreboard::update(std::chrono::milliseconds lapTime, int highestLap)
{
    using namespace std::chrono;

    char buf[128];
    sprintf_s(buf, "Score: %d", highestLap);
    scoreText.setText(buf);
    scoreText.update();

    sprintf_s(buf, "Lap time: %2d:%02lld.%03lld",
        duration_cast<minutes>(lapTime).count(), duration_cast<seconds>(lapTime).count() % 60, lapTime.count() % 1000);
    timerText.setText(buf);
    timerText.update();
}

void Scoreboard::reset()
{
    score = 0;
    lap = 0;
}

void Scoreboard::draw() const
{
    scoreText.draw();
    timerText.draw();
}
