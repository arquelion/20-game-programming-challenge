#include "arena.h"
#include "view.h"

using namespace ci;

void Arena::draw()
{
    auto scaledUpperWallLeft = view.calcScaledVec2(upperWallLeft);
    auto scaledUpperWallRight = view.calcScaledVec2(upperWallRight);
    auto scaledLowerWallLeft = view.calcScaledVec2(lowerWallLeft);
    auto scaledLowerWallRight = view.calcScaledVec2(lowerWallRight);
    auto scaledNetTop = view.calcScaledVec2(netTop);
    auto scaledNetBot = view.calcScaledVec2(netBot);

    gl::color(color);
    gl::context()->pushLineWidth(view.calcScaledValue(arenaStrokeWidth));
    gl::drawLine(scaledUpperWallLeft, scaledUpperWallRight);
    gl::drawLine(scaledNetTop, scaledNetBot);
    gl::drawLine(scaledLowerWallLeft, scaledLowerWallRight);
    gl::context()->popLineWidth();
}
