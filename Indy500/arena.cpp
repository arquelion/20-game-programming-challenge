#include "precomp.h"

#include "arena.h"

using namespace ci;

Arena::Arena()
{

}

void Arena::loadLevel(int level)
{
    terrain.clear();
    data.collideables.clear();

    Object2D block;
    block.color = Color::gray(0.5f);
    block.sprite.setCenter({ 10, 10 });
    block.sprite.setRadius({ 4, 4 });
    terrain.push_back(block);
    //data.collideables.push_back(block.sprite.getBoundingBox());
}

void Arena::update(float deltaSec)
{

}

void Arena::draw() const
{
    for (auto& object : terrain)
    {
        object.draw();
    }
}
