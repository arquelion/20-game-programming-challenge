#include "precomp.h"

#include "game.h"
#include "menuMode.h"
#include "main.h"
#include "playMode.h"
#include "view.h"

using namespace ci;
using namespace ci::app;

View view{ 165, 140 };
GameState game;
float epsilon = 0.001f;
FILE* logFile = nullptr;
int frameCount = 0;

void prepareSettings(IndyClone::Settings* settings)
{
    settings->setMultiTouchEnabled(false);
    settings->setWindowSize(view.getSize() * 5.f);
}

void IndyClone::keyDown(ci::app::KeyEvent event)
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

void IndyClone::setup()
{
    hideCursor();
    addAssetDirectory(".");
    fopen_s(&logFile, "log.txt", "w");
    view.setOrigin(View::Origin::CENTER);
    mode = std::make_unique<PlayMode>(this);
    next = current = IndyClone::GameMode::PLAY;
}

void IndyClone::quit()
{
    App::quit();
}

void IndyClone::update()
{
    view.updateWindow(getWindowWidth() * 1.f, getWindowHeight() * 1.f);
    mode->update();
    updateMode();
    ++frameCount;
}

void IndyClone::draw()
{
    gl::clear(Color::black());
    mode->draw();
}

void IndyClone::updateMode()
{
    if (next != current)
    {
        switch (next)
        {
        case GameMode::IDLE:
            mode = std::make_unique<MenuMode>(this);
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
CINDER_APP(IndyClone, RendererGl, prepareSettings)
