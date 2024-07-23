#pragma once
class Croc;

#include "log.h"

#include "clock.h"

class Croc : public Log
{
public:
    Croc(float speed);

    void update(float deltaSec) override;

    bool isMouthOpen = false;

private:
    float openBoundAdjust;

    ci::gl::Texture2dRef mouthOpenTex;
    ci::gl::Texture2dRef mouthClosedTex;

    Clock::time_point transitionTime;
    Clock::duration openDuration;
    Clock::duration closedDuration;
};
