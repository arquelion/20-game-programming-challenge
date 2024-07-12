#pragma once
#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"

#include "gameState.h"
#include "mode.h"

class Mode;

class PongClone : public ci::app::App {
public:
    enum class GameMode
    {
        END,
        IDLE,
        PLAY,
    };

    void mouseMove(ci::app::MouseEvent event) override;
    void keyDown(ci::app::KeyEvent event) override;

    void setup() override;
    void update() override;
    void draw() override;

    GameMode next;

private:
    GameState game;
    std::unique_ptr<Mode> mode;
    GameMode current;

    void updateMode();
};
