#include "precomp.h"

#include "crocodile.h"

#include "gameState.h"

using namespace ci;

Croc::Croc(float speed)
    : Log(LogSize::MEDIUM, speed)
{
    Surface bitmap(loadImage("croc.png"));

    Area area;
    Surface croc;

    auto radius = sprite.getRadius();
    radius.x = radius.y * 24 / 7;
    sprite.setRadius(radius);
    openBoundAdjust = (sprite.getBotRight().x - sprite.getTopLeft().x) * -1 / 6;

    area = Area(Rectf({ 0, 2 }, { 48, 16 }));
    croc = Surface(48, 14, true);
    croc.copyFrom(bitmap, area, glm::ivec2(0, -2));
    ip::flipHorizontal(&croc);
    mouthOpenTex = gl::Texture2d::create(croc);

    area = Area(Rectf({ 0, 18 }, { 48, 32 }));
    croc = Surface(48, 14, true);
    croc.copyFrom(bitmap, area, glm::ivec2(0, -18));
    ip::flipHorizontal(&croc);
    mouthClosedTex = gl::Texture2d::create(croc);

    texture = mouthClosedTex;

    openDuration = std::chrono::milliseconds(1500);
    closedDuration = std::chrono::milliseconds(500);
}

void Croc::update(float deltaSec)
{
    Log::update(deltaSec);

    if (game.clock.current() > transitionTime)
    {
        isMouthOpen = !isMouthOpen;
        transitionTime = game.clock.current() + (isMouthOpen ? openDuration : closedDuration);
        texture = isMouthOpen ? mouthOpenTex : mouthClosedTex;
        if (isMouthOpen)
        {
            sprite.getBoundingBox().center.x += openBoundAdjust;
            sprite.getBoundingBox().radius.x += openBoundAdjust;
        }
        else
        {
            sprite.getBoundingBox().center = sprite.getCenter();
            sprite.getBoundingBox().radius = sprite.getRadius();
        }
    }
}
