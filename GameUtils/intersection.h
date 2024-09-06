#pragma once
struct AABB;
struct OBB;
extern float epsilon;

#include <optional>
#include <vector>

#include "cinder/gl/gl.h"

#include "utils.h"

/* Sources: https://noonat.github.io/intersect/                                           */
/*          https://textbooks.cs.ksu.edu/cis580/04-collisions/04-separating-axis-theorem/ */

struct Hit
{
    Hit() {}
    Hit(const AABB& box) : box(&box) {}

    const AABB* box = nullptr;
    glm::vec2 pos{ 0, 0 }; // Position of collision
    glm::vec2 delta{ 0, 0 }; // Collision overlap, add to colliding object's position to undo
    glm::vec2 normal{ 0, 0 }; // Surface normal at point of collision
    float t{ 0.f }; // On the interval [0, 1]
};

struct Sweep
{
    std::optional<Hit> hit; // Pointer to Hit object if collision, nullopt otherwise
    glm::vec2 pos{ 0, 0 }; // Furthest point object reached along swept path before collision
    float t{ 1.f }; // Copy of sweep.hit.time offset by epsilon if collision, 1 otherwise
};

// Axis-Aligned Bounding Box
struct AABB
{
    AABB() {}
    AABB(glm::vec2 pos, glm::vec2 radius)
        : center(pos)
        , radius(radius) {}
    bool operator==(const AABB&) const = default;

    // Static vs static collision
    std::optional<Hit> intersectPoint(glm::vec2 point) const;
    std::optional<Hit> intersectSegment(glm::vec2 origin, glm::vec2 dir, glm::vec2 padding = { 0, 0 }) const;
    std::optional<Hit> intersectAABB(AABB object) const;

    // Moving vs static collision
    Sweep sweepAABB(AABB object, glm::vec2 dir = { 0, 0 }) const;
    Sweep sweepInto(std::vector<AABB> objects, glm::vec2 dir = { 0, 0 }) const;

    glm::vec2 center{ 0, 0 }; // Center point
    glm::vec2 radius{ 0, 0 }; // "Radius"
};

// Oriented Bounding Box - TODO: generalize to 3d
struct OBB
{
public:
    OBB() {}
    OBB(glm::vec2 center, std::vector<glm::vec2> vertices);
    OBB(glm::vec2 center, glm::vec2 radius);

    bool isColliding(const OBB& that) const;
    Sweep sweepOBB(const OBB& dynamicObj, glm::vec2 dir = { 0, 0 }) const;
    Sweep sweepOBBImpl(const OBB& dynamicObj, glm::vec2 dir) const;
    glm::vec2 support(const OBB& that, glm::vec2 dir) const;

    void rotate(float radians);
    void translate(glm::vec2 dir);

    glm::vec2 center{ 0, 0 };
    std::vector<glm::vec2> vertices;
    std::vector<glm::vec2> normals;

private:
    void calcNormals();
    std::pair<float, float> project(glm::vec2 axis) const;

    glm::vec2 findFurthestPoint(glm::vec2 dir) const;
};

namespace std
{
    template<> struct hash<glm::vec2>
    {
        typedef glm::vec2 argument_type;
        typedef std::size_t result_type;
        result_type operator()(argument_type const& vec) const noexcept
        {
            return std::hash<float>{}(vec.x) ^ std::hash<float>{}(vec.y);
        }
    };

    template<> struct hash<AABB>
    {
        typedef AABB argument_type;
        typedef std::size_t result_type;
        result_type operator()(argument_type const& aabb) const noexcept
        {
            return std::hash<glm::vec2>{}(aabb.center);
        }
    };
}
