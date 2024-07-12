#include "precomp.h"

#include "endMode.h"
#include "game.h"
#include "idleMode.h"
#include "main.h"
#include "playMode.h"
#include "view.h"

using namespace ci;
using namespace ci::app;

View view{ 140, 165 };
GameState game;
float epsilon = 0.001f;
FILE* logFile = nullptr;
int frameCount = 0;

void prepareSettings(FroggerClone::Settings* settings)
{
    settings->setMultiTouchEnabled(false);
    settings->setWindowSize(glm::vec2(700, 825));
}

void FroggerClone::keyDown(ci::app::KeyEvent event)
{
    if (event.getCode() == KeyEvent::KEY_ESCAPE) {
        // Exit full screen, or quit the application, when the user presses the ESC key.
        if (isFullScreen())
            setFullScreen(false);
        else
            quit();
    }
    else
    {
        mode->keyDown(event);
    }
}

void FroggerClone::setup()
{
    hideCursor();
    setFrameRate(144);
    fopen_s(&logFile, "log.txt", "w");
    mode = std::make_unique<PlayMode>(this);
    next = current = FroggerClone::GameMode::PLAY;

    /*std::ifstream highScoreFile{"highscores.txt", std::ifstream::in};

    int numEntries;
    highScoreFile >> numEntries;

    if (highScoreFile.good())
    {
        for (int row = 0; row < numEntries && row < 5; ++row)
        {
            highScoreFile >> game.highScores[row];
        }
    }*/
}

void FroggerClone::quit()
{
    /*std::ofstream highScoreFile{"highscores.txt", std::ofstream::out};

    if (highScoreFile.good())
    {
        highScoreFile << "5" << "\n";
        for (int i = 0; i < 5; ++i)
        {
            highScoreFile << game.highScores[i] << "\n";
        }
    }*/

    App::quit();
}

void FroggerClone::update()
{
    view.updateWindow(getWindowWidth() * 1.f, getWindowHeight() * 1.f);
    mode->update();
    updateMode();
    ++frameCount;
}

void FroggerClone::draw()
{
    gl::clear(Color::black());
    mode->draw();
}

void FroggerClone::updateMode()
{
    if (next != current)
    {
        switch (next)
        {
        case GameMode::END:
            mode = std::make_unique<EndMode>(this);
            break;
        case GameMode::IDLE:
            mode = std::make_unique<IdleMode>(this);
            break;
        case GameMode::PLAY:
            mode = std::make_unique<PlayMode>(this);
            break;
        default:
            throw std::exception("unknown game mode");
        }
    }
    current = next;
}

// This line tells Cinder to actually create and run the application.
CINDER_APP(FroggerClone, RendererGl, prepareSettings)
