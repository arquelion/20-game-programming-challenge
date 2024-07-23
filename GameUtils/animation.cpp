#include "animation.h"
#include "view.h"

using namespace ci;

Animation::Animation(Clock& clock)
    : clock(clock)
{
}

bool Animation::isPlaying() const
{
    return isPlaying_;
}

void Animation::play(glm::vec2 start, glm::vec2 end)
{
    if (!isPlaying())
    {
        startPos = start;
        endPos = end;
        startTime = clock.now();

        isPlaying_ = true;
        frameIndex = 0;
    }
}

bool Animation::update()
{
    bool stoppedPlaying = false;
    if (isPlaying_)
    {
        isPlaying_ = clock.now() - startTime < duration;
        stoppedPlaying = !isPlaying_;
    }

    if (isPlaying_)
    {
        auto progress = (clock.now() - startTime).count() / (float)duration.count();
        frameIndex = (int)(progress * frames.size());
        if (frameIndex >= frames.size() || frameIndex < 0)
        {
            throw std::runtime_error("invalid frame index");
        }

        container.offsetCenterTo(lerp(startPos, endPos, progress));
    }

    return stoppedPlaying;
}

void Animation::draw() const
{
    if (isPlaying_)
    {
        gl::color(Color::white());
        gl::draw(frames[frameIndex], view.getRect(container));
    }
}
