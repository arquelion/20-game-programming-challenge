#pragma once
class Turtle;

#include "clock.h"
#include "object2D.h"

enum SinkState
{
    EMERGED = 3,
    SUBMERGED = 0,
};

enum class TurtlePod
{
    DOUBLE,
    TRIPLE,
};

class Turtle : public Object2D
{
public:
    Turtle(TurtlePod count, float speed, Clock::duration sinkRate);

    void update() override;

private:
    TurtlePod count;
    Clock::duration sinkRate;

    int sinkState;
    Clock::time_point stateStartTime;
    Clock::duration stateDuration;
    bool isSubmerging = true;
};
