#include "mousePlayer.h"
#include "utils.h"
#include "view.h"

void MousePlayer::move(glm::vec2 pos, std::chrono::steady_clock::time_point time)
{
    glm::vec2 velocity{ 0, 0 };
    auto distance = (pos - lastPos).y * view.getPixelToDesignScale().y;
    auto timeElapsed = float(std::chrono::duration_cast<std::chrono::microseconds>(time - lastTimeMoved).count() / 1e6);

    if (timeElapsed > 1e-6)
    {
        paddle.mouseMove(distance, timeElapsed);

        lastPos = pos;
        lastTimeMoved = time;
    }
}
