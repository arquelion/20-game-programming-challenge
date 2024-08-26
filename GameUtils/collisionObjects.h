#pragma once
class Collideable2D;
struct NetCollideable2D;
class BoundedRect;

#include "cinder/gl/gl.h"

#include "intersection.h"
#include "renderObjects.h"

class Collideable2D
{
public:
    Collideable2D(glm::vec2 center, glm::vec2 radius);
    auto operator=(const NetCollideable2D source) -> Collideable2D;

    void translate(glm::vec2 dir);
    void rotate(Radians delta);

    bool isOffscreen() const;
    void reset();

    void update(float deltaSec);
    void draw() const;

    OBB boundingBox;
    Renderable2D renderObj;
};

struct NetCollideable2D
{
    auto operator=(const Collideable2D source) -> NetCollideable2D;

    OBB boundingBox;
    NetRenderable2D renderObj;
};

// Used for Frogger only
class BoundedRect
{
public:
    BoundedRect() {}
    static BoundedRect create(glm::vec2 center, glm::vec2 radius);
    static BoundedRect createFromCorners(glm::vec2 topLeft, glm::vec2 botRight);

    glm::vec2 getTopLeft() const { return center - radius; }
    glm::vec2 getBotRight() const { return center + radius; }
    glm::vec2& getCenter() { return center; }
    glm::vec2& getRadius() { return radius; }
    AABB& getBoundingBox() { return boundingBox; }

    void setCenter(glm::vec2 newCenter) { boundingBox.center = center = newCenter; }
    void setRadius(glm::vec2 newRadius) { boundingBox.radius = radius = newRadius; }

    const glm::vec2& getCenter() const { return center; }
    const glm::vec2& getRadius() const { return radius; }
    const AABB& getBoundingBox() const { return boundingBox; }

    void translate(glm::vec2 dir);

private:
    BoundedRect(glm::vec2 center, glm::vec2 radius)
        : center(center)
        , radius(radius)
        , boundingBox(center, radius)
    {}

    glm::vec2 center;
    glm::vec2 radius;

    AABB boundingBox;
};
