#pragma once
class Renderable2D;
struct NetRenderable2D;

#include "cinder/gl/gl.h"

#include "utils.h"

class Renderable2D
{
public:
    Renderable2D();
    Renderable2D(glm::vec2 center, glm::vec2 radius);
    auto operator=(const NetRenderable2D& source) -> Renderable2D&;

    void translate(glm::vec2 dir);
    void rotate(Radians delta);

    bool isOffscreen() const;
    void reset();

    void update(float deltaSec);
    void draw() const;

    ci::Rectf sprite;
    glm::vec2 velocity;
    Radians heading;

    ci::Color color;
    ci::gl::Texture2dRef texture;
    //Animation anim;

    glm::vec2 initialCenter;
    Radians initialHeading;

private:
};

struct NetRenderable2D
{
    auto operator=(const Renderable2D& source) -> NetRenderable2D&;

    ci::Rectf sprite;
    glm::vec2 velocity;
    Radians heading;

    glm::vec2 initialCenter;
    Radians initialHeading;
};