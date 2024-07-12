#pragma once
class Log;

#include "object2D.h"

enum class LogSize
{
    MEDIUM,
    LARGE,
    SMALL,
};

class Log : public Object2D
{
public:
    Log(LogSize size, float speed);

private:
    LogSize size;
};
