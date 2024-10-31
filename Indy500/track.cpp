#include "precomp.h"

#include "track.h"
#include "view.h"

using namespace ci;

Track::Track()
{

}

void Track::loadLevel(int level)
{
    terrain.clear();
    data.collideables.clear();

    Collideable2D block({ 0, 0 }, { 50, 40 });
    block.renderObj.color = Color::gray(0.5f);
    terrain.push_back(block);
    data.collideables.push_back(block.boundingBox);

    data.startingLine.center = { 30, 30 };

    data.beacons.clear();
    OBB center = { {0, 0}, {1, 1} };
    for (int i = 0; i < 8; ++i)
    {
        data.beacons.push_back(center);
        data.beacons.back().translate((65.f + ((i % 2) ? 10.f : 0.f)) * getUnitVector(-i * M_PI / 4));
    }
}

void Track::update(float deltaSec)
{

}

void Track::draw() const
{
    auto startLine = data.startingLine;
    gl::drawLine(startLine.center, startLine.center + getUnitVector(startLine.angle) * glm::l1Norm(glm::vec3(view.getSize(), 0)));
    for (auto& object : terrain)
    {
        object.draw();
    }
}
