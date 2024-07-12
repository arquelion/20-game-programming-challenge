#include <chrono>
#include <cstdio>

#include "cinder/audio/Voice.h"

#include "endMode.h"
#include "main.h"
#include "idleMode.h"
#include "playMode.h"
#include "view.h"

using namespace ci;
using namespace ci::app;

View view{ 160, 90 };
FILE* logFile = nullptr;
int frameCount = 0;

void prepareSettings(PongClone::Settings* settings)
{
    settings->setMultiTouchEnabled(false);
    //settings->setFullScreen(true);
}

void PongClone::mouseMove(MouseEvent event)
{
    mode->mouseMove(event);
}

void PongClone::keyDown(KeyEvent event)
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

void PongClone::setup()
{
    addAssetDirectory("C:/Users/lucy9/game-programming/20-game-challenge/Pong");
    audio::SourceFileRef sourceFile = audio::load(app::loadAsset("34337__shammons__organ_button.wav"));
    game.bounceSound = audio::Voice::create(sourceFile);
    hideCursor();
    fopen_s(&logFile, "D:/Users/Lucy C/Documents/log.txt", "w");
    mode = std::make_unique<IdleMode>(this, game);
    next = current = PongClone::GameMode::IDLE;   
}

void PongClone::update()
{
    view.updateWindow(getWindowWidth() * 1.f, getWindowHeight() * 1.f);
    mode->update();
    updateMode();
    ++frameCount;
}

void PongClone::draw()
{
    gl::clear(Color::gray(0.5f));

    auto offsetRects = view.getOffsetRects();
    gl::color(Color(0.f, 0.f, 0.f));
    gl::drawSolidRect(offsetRects[0]);
    gl::drawSolidRect(offsetRects[1]);

    mode->draw();
}

void PongClone::updateMode()
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
CINDER_APP( PongClone, RendererGl, prepareSettings )
