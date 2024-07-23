#include "arena.h"
#include "view.h"

using namespace ci;

void Arena::draw()
{
    auto scaledUpperWallLeft = view.getViewVec2(upperWallLeft);
    auto scaledUpperWallRight = view.getViewVec2(upperWallRight);
    auto scaledLowerWallLeft = view.getViewVec2(lowerWallLeft);
    auto scaledLowerWallRight = view.getViewVec2(lowerWallRight);
    auto scaledNetTop = view.getViewVec2(netTop);
    auto scaledNetBot = view.getViewVec2(netBot);

    gl::color(color);
    gl::context()->pushLineWidth(view.calcScaledValue(arenaStrokeWidth));
    gl::drawLine(scaledUpperWallLeft, scaledUpperWallRight);
    gl::drawLine(scaledNetTop, scaledNetBot);
    gl::drawLine(scaledLowerWallLeft, scaledLowerWallRight);
    gl::context()->popLineWidth();
}
