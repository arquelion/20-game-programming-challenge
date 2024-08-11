#include <numeric>

#include "animation.h"
#include "view.h"

using namespace ci;

Animation::Animation(Clock& clock)
    : clock(clock)
    , frameIndex(0)
{
}

void Animation::setDuration(Clock::duration duration)
{
    this->durations.resize(1);
    this->durations[0] = duration;
    isSimpleDuration = durations.size() != frames.size();
    totalDuration = duration * frames.size();
}

void Animation::setDuration(std::vector<Clock::duration> durations)
{
    this->durations = durations;
    isSimpleDuration = durations.size() != frames.size();
    totalDuration = std::reduce(std::begin(durations), std::end(durations));
}

void Animation::addFrameBehavior(int frame, std::function<void()> func)
{
    if (frame < frames.size())
    {
        behaviors.resize(frames.size());
        behaviors[frame] = func;
    }
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
        startTime = clock.current();

        isPlaying_ = true;
        frameIndex = 0;
    }
}

void Animation::loop()
{
    isLooping_ = true;
    play(startPos, endPos);
}

bool Animation::update()
{
    bool stoppedPlaying = false;
    auto duration = isSimpleDuration
        ? durations[0]
        : durations[frameIndex];
    auto elapsedDuration = clock.current() - startTime;
    
    if (isPlaying_ && !isLooping_)
    {
        isPlaying_ = elapsedDuration < totalDuration;
        stoppedPlaying = !isPlaying_;
    }

    if (isPlaying_)
    {
        auto progress = (float)elapsedDuration.count() / totalDuration.count();
        if (isLooping_)
        {
            float integral;
            progress = ::modf(progress, &integral);
        }

        int newFrameIndex = 0;
        if (isSimpleDuration)
        {
            newFrameIndex = (int)(progress * frames.size());
        }
        else
        {
            int index = 0;
            newFrameIndex = (int)frames.size() - 1;
            while (elapsedDuration.count() > 0)
            {
                if (elapsedDuration.count() < durations[index].count())
                {
                    newFrameIndex = index;
                    break;
                }
                elapsedDuration -= durations[index];
                ++index;
                if (isLooping_)
                {
                    index %= frames.size();
                }
            }
        }

        if (newFrameIndex >= frames.size() || newFrameIndex < 0)
        {
            throw std::runtime_error("invalid frame index");
        }

        if (newFrameIndex != frameIndex)
        {
            frameIndex = newFrameIndex;
            if (behaviors[frameIndex])
            {
                behaviors[frameIndex]();
            }
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
