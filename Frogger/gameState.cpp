#include "precomp.h"

#include "gameState.h"

void GameState::init()
{
    arena = std::make_shared<Arena>();
    frog = std::make_shared<Frog>();
    scoreboard = std::make_shared<Scoreboard>();
}

void GameState::newGame()
{
    // TODO: placeholder to get things working
    arena->loadLevel(1);
}

bool GameState::isGameOver() const
{
    return livesRemaining <= 0;
}

void GameState::loseLife()
{
    if (!frog->deathAnim.isPlaying())
    {
        --livesRemaining;
        frog->animate(true);
        scoreboard->update();
    }
}

void GameState::update(float deltaSec)
{
    if (isActive)
    {
        scoreboard->update();

        if (frogsRemaining <= 0)
        {
            // TODO: advance to next level
            frogsRemaining = 5;
            frog->reset();
        }

        frog->update();
        arena->update(deltaSec);

        if (frog->isInHouse())
        {
            --frogsRemaining;
            scoreboard->incrementScore(ScoreSource::HOUSE);
            if (frogsRemaining <= 0)
                return;
            frog->reset();
        }
        else if (frog->isDead() || clock.now() >= frogTimeLimit)
        {
            loseLife();
        }
        else
        {
            if (frog->reachedNewRow)
            {
                scoreboard->incrementScore(ScoreSource::MOVEMENT);
                frog->reachedNewRow = false;
            }
        }
    }
}

void GameState::draw() const
{
    arena->draw();
    scoreboard->draw();
    frog->draw();
}
