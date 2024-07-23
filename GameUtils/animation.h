#pragma once
class Animation;

#include <vector>

#include "cinder/gl/gl.h"

#include "clock.h"

class Animation
{
public:
    Animation(Clock& clock);

    bool isPlaying() const;

    void play(glm::vec2 start, glm::vec2 end);
    bool update();
    void draw() const;

    std::vector<ci::gl::Texture2dRef> frames;
    int frameIndex;

    Clock::duration duration;

    ci::Rectf container;

private:
    glm::vec2 startPos;
    glm::vec2 endPos;

    Clock& clock;
    Clock::time_point startTime;

    bool isPlaying_ = false;
};
