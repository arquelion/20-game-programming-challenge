#include <fstream>

#include "cinder/audio/Voice.h"

#include "endMode.h"
#include "idleMode.h"
#include "main.h"
#include "playMode.h"
#include "view.h"

using namespace ci;
using namespace ci::app;

View view{ 100, 75 };
float epsilon = 0.001f;
FILE* logFile = nullptr;
int frameCount = 0;

void prepareSettings(BreakoutClone::Settings* settings)
{
    settings->setMultiTouchEnabled(false);
}

void BreakoutClone::mouseMove(MouseEvent event)
{
    mode->mouseMove(event);
}

void BreakoutClone::keyDown(ci::app::KeyEvent event)
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

void BreakoutClone::mouseDown(ci::app::MouseEvent event)
{
    mode->mouseDown(event);
}

void BreakoutClone::setup()
{
    hideCursor();
    setFrameRate(144);
    fopen_s(&logFile, "log.txt", "w");
    mode = std::make_unique<IdleMode>(this, game);
    next = current = BreakoutClone::GameMode::IDLE;

    addAssetDirectory(".");
    audio::SourceFileRef sourceFile = audio::load(app::loadAsset("34337__shammons__organ_button.wav"));
    game.bounceSound = audio::Voice::create(sourceFile);

    std::ifstream highScoreFile{ "highscores.txt", std::ifstream::in };

    int numEntries;
    highScoreFile >> numEntries;

    if (highScoreFile.good())
    {
        for (int row = 0; row < numEntries && row < 5; ++row)
        {
            highScoreFile >> game.highScores[row];
        }
    }
}

void BreakoutClone::quit()
{
    std::ofstream highScoreFile{ "highscores.txt", std::ofstream::out };

    if (highScoreFile.good())
    {
        highScoreFile << "5" << "\n";
        for (int i = 0; i < 5; ++i)
        {
            highScoreFile << game.highScores[i] << "\n";
        }
    }

    App::quit();
}

void BreakoutClone::update()
{
    view.updateWindow(getWindowWidth() * 1.f, getWindowHeight() * 1.f);
    mode->update();
    updateMode();
    ++frameCount;
}

void BreakoutClone::draw()
{
    gl::clear(Color::black());
    mode->draw();
}

void BreakoutClone::updateMode()
{
    if (next != current)
    {
        switch (next)
        {
        case GameMode::END:
            mode = std::make_unique<EndMode>(this, game);
            break;
        case GameMode::IDLE:
            mode = std::make_unique<IdleMode>(this, game);
            break;
        case GameMode::PLAY:
            mode = std::make_unique<PlayMode>(this, game);
            break;
        default:
            throw std::exception("unknown game mode");
        }
    }
    current = next;
}

// This line tells Cinder to actually create and run the application.
CINDER_APP(BreakoutClone, RendererGl, prepareSettings)
