#pragma once
class Frog;

#include "game.h"

#include "animation.h"
#include "intersection.h"

class Frog
{
public:
    Frog();

    bool isDead();
    bool isInHouse() const;

    void animate(bool isDead);
    void move(Direction dir);
    void reset();
    void update();
    void draw() const;

    static const glm::vec2 collisionRadius;
    bool reachedNewRow = false;

    Animation deathAnim;

private:
    BoundedRect sprite;
    ci::gl::Texture2dRef texture;
    glm::vec2 nextDir;
    const glm::vec2 initialCenter{ 75, 85 };
    const glm::vec2 initialRadius{ 4, 4 };

    float farthestYReached = initialCenter.y;
};
