#include "intersection.h"

std::optional<Hit> AABB::intersectPoint(glm::vec2 point) const
{
    auto dist = point - center;
    auto pDist = radius - glm::abs(dist);
    if (pDist.x < 0 || pDist.y < 0)
        return std::nullopt;

    auto hit = std::make_optional<Hit>(*this);
    if (pDist.x < pDist.y)
    {
        auto sign = signum(dist.x);
        hit->delta.x = pDist.x * sign;
        hit->normal.x = sign;
        hit->pos = { center.x + (radius.x * sign), point.y };
    }
    else
    {
        auto sign = signum(dist.y);
        hit->delta.y = pDist.y * sign;
        hit->normal.y = sign;
        hit->pos = { point.x, center.y + (radius.y * sign) };
    }

    return hit;
}

std::optional<Hit> AABB::intersectSegment(glm::vec2 origin, glm::vec2 dir, glm::vec2 padding) const
{
    glm::vec2 scale{ 1.f / dir };
    glm::vec2 sign{ signum(scale.x), signum(scale.y) };
    glm::vec2 t1 = (center - sign * (radius + padding) - origin) * scale; // nearTimeX, nearTimeY
    glm::vec2 t2 = (center + sign * (radius + padding) - origin) * scale; // farTimeX, farTimeY

    if (t1.x > t2.y || t1.y > t2.x)
        return std::nullopt;

    auto nearTime = std::max(t1.x, t1.y);
    auto farTime = std::min(t2.x, t2.y);
    if (nearTime > 1.f || farTime <= 0.f)
        return std::nullopt;

    auto hit = std::make_optional<Hit>(*this);
    hit->t = std::clamp(nearTime, 0.f, 1.f);
    if (t1.x > t1.y)
    {
        hit->normal = { -sign.x, 0.f };
    }
    else
    {
        hit->normal = { 0.f, -sign.y };
    }
    hit->delta = (1.f - hit->t) * -dir;
    hit->pos = origin + dir * hit->t;

    return hit;
}

std::optional<Hit> AABB::intersectAABB(AABB object) const
{
    auto dist = object.center - center;
    auto pDist = (object.radius + radius) - glm::abs(dist);
    if (pDist.x < 0 || pDist.y < 0)
        return std::nullopt;

    Hit hit{ *this };
    glm::vec2 sign = { signum(dist.x), signum(dist.y) };
    if (pDist.x < pDist.y)
    {
        hit.delta.x = pDist.x * sign.x;
        hit.normal.x = sign.x;
        hit.pos = { center.x + (radius.x * sign.x), object.center.y };
    }
    else
    {
        hit.delta.y = pDist.y * sign.y;
        hit.normal.y = sign.y;
        hit.pos = { object.center.x, center.y + (radius.y * sign.y) };
    }

    return std::make_optional(hit);
}

Sweep AABB::sweepAABB(AABB object, glm::vec2 dir) const
{
    Sweep sweep;

    if (dir.x == 0 && dir.y == 0)
    {
        sweep.pos = object.center;
        sweep.hit = intersectAABB(object);
        sweep.t = sweep.hit ? (sweep.hit->t = 0.f) : 1.f;
        return sweep;
    }

    sweep.hit = intersectSegment(object.center, dir, object.radius);
    if (sweep.hit)
    {
        sweep.t = std::clamp(sweep.hit->t - epsilon, 0.f, 1.f);
        sweep.pos = object.center + dir * sweep.t;
        auto dHat = glm::normalize(dir);
        sweep.hit->pos = glm::clamp(sweep.hit->pos + dHat * object.radius, center - radius, center + radius);
    }
    else
    {
        sweep.pos = object.center + dir;
        sweep.t = 1.f;
    }

    return sweep;
}

Sweep AABB::sweepInto(std::vector<AABB> objects, glm::vec2 dir) const
{
    Sweep nearest;
    nearest.t = 1.f;
    nearest.pos = center + dir;
    for (auto& object : objects)
    {
        auto sweep = object.sweepAABB(*this, dir);
        if (sweep.t < nearest.t)
            nearest = sweep;
    }

    return nearest;
}

BoundedRect BoundedRect::create(glm::vec2 center, glm::vec2 radius)
{
    return BoundedRect(center, radius);
}

BoundedRect BoundedRect::createFromCorners(glm::vec2 topLeft, glm::vec2 botRight)
{
    glm::vec2 center = (topLeft + botRight) / 2.f;
    glm::vec2 radius = center - topLeft;
    return BoundedRect(center, radius);
}

void BoundedRect::translate(glm::vec2 dir)
{
    center += dir;
    boundingBox.center += dir;
}
