#include "gameState.h"

using namespace ci;

template <typename T>
void removeDeletedElements(std::vector<T>& vec, std::vector<int> indicesToDelete)
{
    std::sort(indicesToDelete.begin(), indicesToDelete.end(), std::greater<int>());
    for (auto& index : indicesToDelete)
    {
        std::erase(vec, vec[index]);
    }
}

void GameState::init()
{
    arena = std::make_shared<Arena>(*this);
    scoreboard = std::make_shared<Scoreboard>();

    paddle = std::make_shared<Paddle>(*this, glm::vec2(50, 70));
    balls.clear();
    balls.push_back(std::make_shared<Ball>(*this));

    levelState = LevelState::PLAY;
    transitionTime = clock.current() + std::chrono::milliseconds(100);

    readyMsg.setText("READY");
}

void GameState::newGame()
{
    scoreboard->reset(highScores[0]);
    balls.clear();
    balls.push_back(std::make_shared<Ball>(*this));
    paddle->reset();
    bricksBeforeNextPowerup = (int)std::ceil(getRandomNum<float>() * 15.f);
    isActive = true;
}

bool GameState::isGameOver()
{
    return scoreboard->getLives() == 0 || levelsRemaining == 0;
}

void GameState::loseLife()
{
    scoreboard->decrementLives();
    deactivatePowerup();
    fallingPowerups.clear();
    paddle->reset();
    balls.push_back(std::make_shared<Ball>(*this));
}

void GameState::update(float deltaSec)
{
    readyMsg.update();

    if (isActive)
    {
        scoreboard->update();

        if (arena->isLevelClear())
        {
            if (paddle->wentThroughBonusExit)
            {
                scoreboard->incrementScore(10000);
            }
            --levelsRemaining;
            ++currentLevel;
            arena->loadLevel(currentLevel);
            lasers.clear();
            fallingPowerups.clear();
            deactivatePowerup();
            balls.clear();
            balls.push_back(std::make_shared<Ball>(*this));
            paddle->reset();
            transitionTime = clock.current();
        }

        if (clock.current() >= transitionTime)
        {
            switch (levelState)
            {
            case LevelState::LOAD:
                levelState = LevelState::READY;
                transitionTime = clock.current() + std::chrono::milliseconds(2000);
                break;
            case LevelState::READY:
                levelState = LevelState::PLAY;
                transitionTime = clock.current() + std::chrono::days(1);
                break;
            case LevelState::PLAY:
                levelState = LevelState::LOAD;
                transitionTime = clock.current() + std::chrono::milliseconds(100);
                break;
            default:
                break;
            }
        }
    }

    std::vector<int> indicesToDelete;
    
    for (int index = 0; auto& powerup : fallingPowerups)
    {
        if (powerup.hasExited())
        {
            indicesToDelete.push_back(index);
        }
        else
        {
            powerup.move(deltaSec);

            if (powerup.isPickedUp(*paddle))
            {
                activateNewPowerup(powerup.getType());
                std::erase(fallingPowerups, powerup);
            }
        }
        ++index;
    }
    removeDeletedElements(fallingPowerups, indicesToDelete);
    indicesToDelete.clear();

    for (int index = 0; auto& laserPair : lasers)
    {
        if (laserPair.first.hasExited() || laserPair.second.hasExited())
        {
            indicesToDelete.push_back(index);
        }
        else
        {
            laserPair.first.move(deltaSec);
            laserPair.second.move(deltaSec);
        }
        ++index;
    }
    removeDeletedElements(lasers, indicesToDelete);
    indicesToDelete.clear();

    for (int index = 0; auto ball : balls)
    {
        if (ball->hasExited())
        {
            indicesToDelete.push_back(index);
        }
        else
        {
            ball->move(deltaSec);
            if (ball->center.x >= paddle->center.x - paddle->radius.x - ball->radius
                && ball->center.x <= paddle->center.x + paddle->radius.x + ball->radius
                && ball->center.y >= paddle->center.y - paddle->radius.y - ball->radius
                && ball->center.y <= paddle->center.y + paddle->radius.y + ball->radius
                && isActive && !ball->isAttached)
            {
                throw std::logic_error("ball in paddle");
            }
        }
        ++index;
    }
    removeDeletedElements(balls, indicesToDelete);
    indicesToDelete.clear();

    if (balls.size() == 1 && activePowerup == PowerupType::DISRUPTION)
    {
        deactivatePowerup();
    }
    else if (balls.size() <= 0)
    {
        loseLife();
    }
}

void GameState::draw()
{
    if (levelState == LevelState::PLAY)
    {
        for (auto ball : balls)
        {
            ball->draw();
        }
    }

    arena->draw();

    if (isActive)
    {
        scoreboard->draw();
        if (levelState == LevelState::PLAY)
        {
            for (auto& powerup : fallingPowerups)
            {
                powerup.draw();
            }
            for (auto& laserPair : lasers)
            {
                laserPair.first.draw();
                laserPair.second.draw();
            }
            paddle->draw();
        }
        if (levelState == LevelState::READY)
        {
            readyMsg.draw();
        }
    }
}

void GameState::activateNewPowerup(PowerupType newType)
{
    if (newType == activePowerup) return;

    deactivatePowerup();
    activePowerup = newType;

    switch (activePowerup)
    {
    case PowerupType::NONE:
        return;
    case PowerupType::LASER:
        paddle->isLaserEnabled = true;
        paddle->inner = Color(0.5f, 0.5f, 0.5f);
        paddle->middle = Color(0.8f, 0.2f, 0.2f);
        paddle->outer = Color(0.5f, 0.5f, 0.5f);
        break;
    case PowerupType::ENLARGE:
        paddle->radius.x *= 1.5f;
        paddle->boundingBox.radius = paddle->radius;
        // Push paddle back in bounds
        paddle->move(paddle->center);
        break;
    case PowerupType::CATCH:
        paddle->isCatchEnabled = true;
        break;
    case PowerupType::BREAK:
        arena->isBonusExitOpen = true;
        break;
    case PowerupType::DISRUPTION:
    {
        arePowerupsEnabled = false;
        fallingPowerups.clear();

        auto newBall = std::make_shared<Ball>(*this);
        newBall->reset();
        newBall->center = balls[0]->center;
        newBall->isAttached = false;
        balls.push_back(newBall);

        newBall = std::make_shared<Ball>(*this);
        newBall->reset();
        newBall->center = balls[0]->center;
        newBall->isAttached = false;
        balls.push_back(newBall);

        break;
    }
    case PowerupType::PLAYER:
        scoreboard->incrementLives();
        activePowerup = PowerupType::NONE;
        break;
    default:
        throw std::logic_error("nonexistent powerup");
    }
}

void GameState::deactivatePowerup()
{
    switch (activePowerup)
    {
    case PowerupType::NONE:
    case PowerupType::PLAYER:
        return;
    case PowerupType::LASER:
        paddle->isLaserEnabled = false;
        paddle->inner = paddle->initialInner;
        paddle->middle = paddle->initialMiddle;
        paddle->outer = paddle->initialOuter;
        break;
    case PowerupType::ENLARGE:
        paddle->radius = paddle->initialRadius;
        paddle->boundingBox.radius = paddle->radius;
        break;
    case PowerupType::CATCH:
        paddle->isCatchEnabled = false;
        if (!balls.empty())
        {
            balls[0]->release(); // Catch and Disruption cannot be active at the same time, so there should only be one ball.
        }
        break;
    case PowerupType::BREAK:
        arena->isBonusExitOpen = false;
        break;
    case PowerupType::DISRUPTION:
        arePowerupsEnabled = true;
        break;
    default:
        throw std::logic_error("nonexistent powerup");
    }

    activePowerup = PowerupType::NONE;
}
