#pragma once
#include "mode.h"

/* SFX Generator: https://sfxr.me/ */

class IndyClone : public ci::app::App {
public:
    enum class GameMode
    {
        END,
        IDLE,
        PLAY,
    };

    void keyDown(ci::app::KeyEvent event) override;

    void setup() override;
    void quit() override;
    void update() override;
    void draw() override;

    GameMode next;

private:
    std::unique_ptr<Mode> mode;
    GameMode current;

    void updateMode();
};
