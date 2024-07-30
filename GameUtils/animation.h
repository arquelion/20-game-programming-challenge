#pragma once
class Animation;

#include <vector>

#include "cinder/gl/gl.h"

#include "clock.h"

class Animation
{
public:
    Animation(Clock& clock);

    void setDuration(Clock::duration duration);
    void setDuration(std::vector<Clock::duration> durations);

    void addFrameBehavior(int frame, std::function<void()> func);

    bool isPlaying() const;
    void play(glm::vec2 start, glm::vec2 end);
    void loop();

    bool update();
    void draw() const;

    int frameIndex;
    std::vector<ci::gl::Texture2dRef> frames;
    std::vector<Clock::duration> durations;

    ci::Rectf container;

private:
    glm::vec2 startPos;
    glm::vec2 endPos;

    Clock& clock;
    Clock::time_point startTime;
    bool isSimpleDuration = true;
    Clock::duration totalDuration;

    bool isLooping_ = false;
    bool isPlaying_ = false;

    std::vector<std::function<void()>> behaviors;
};
