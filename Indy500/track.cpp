#include "precomp.h"

#include "track.h"

using namespace ci;

Track::Track()
{

}

void Track::loadLevel(int level)
{
    terrain.clear();
    data.collideables.clear();

    Object2D block;
    block.color = Color::gray(0.5f);
    block.sprite.setCenter({ 0, 0 });
    block.sprite.setRadius({ 50, 40 });
    terrain.push_back(block);
    //data.collideables.push_back(block.sprite.getBoundingBox());
}

void Track::update(float deltaSec)
{

}

void Track::draw() const
{
    for (auto& object : terrain)
    {
        object.draw();
    }
}
