#include "intersection.h"
#include "gjk.h"

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

OBB::OBB(glm::vec2 center, std::vector<glm::vec2> vertices)
    : center(center)
    , vertices(vertices)
{
    calcNormals();
}

OBB::OBB(glm::vec2 center, glm::vec2 radius)
    : center(center)
{
    vertices.push_back(center - radius);
    vertices.push_back({ center.x + radius.x, center.y - radius.y });
    vertices.push_back(center + radius);
    vertices.push_back({ center.x - radius.x, center.y + radius.y });
    calcNormals();
}

bool OBB::isColliding(const OBB& that) const
{
    auto testNormals = [this, &that](auto normals) {
        for (auto& normal : normals)
        {
            auto p1 = this->project(normal);
            auto p2 = that.project(normal);
            if (p1.second < p2.first || p2.second < p1.first)
                return false;
        }
        return true;
    };

    return testNormals(this->normals) && testNormals(that.normals);
}

Sweep OBB::sweepOBBImpl(const OBB& dynamicObj, glm::vec2 dir) const
{
    Sweep sweep;

    if (dir.x == 0 && dir.y == 0)
    {
        sweep.pos = dynamicObj.center;
        sweep.t = isColliding(dynamicObj) ? 0.f : 1.f;
        return sweep;
    }

    OBB futureObj = dynamicObj;
    futureObj.translate(dir);
    if (!isColliding(futureObj))
    {
        sweep.pos = futureObj.center;
        sweep.t = 1.f;
        return sweep;
    }

    float start = 0;
    float end = 1;
    while (end - start > epsilon)
    {
        float mid = (start + end) / 2;
        futureObj = dynamicObj;
        futureObj.translate(dir * mid);
        if (isColliding(futureObj))
        {
            end = mid;
        }
        else
        {
            start = mid;
        }
    }
    sweep.t = (start + end) / 2 - epsilon;
    sweep.pos = dynamicObj.center + dir * sweep.t;
    return sweep;
}

Sweep OBB::sweepOBB(const OBB& dynamicObj, glm::vec2 dir) const
{
    auto sweep = sweepOBBImpl(dynamicObj, dir);

    bool isCollision = GJK(*this, dynamicObj);

    if ((sweep.t != 1.f) != isCollision)
    {
        throw std::exception("someone screwed up their math");
    }
    return sweep;
}

void OBB::rotate(float radians)
{
    for (auto& v : vertices)
    {
        v = center + glm::rotate(v - center, radians);
    }
    calcNormals();
}

void OBB::translate(glm::vec2 dir)
{
    center += dir;
    for (auto& v : vertices)
    {
        v += dir;
    }
}

void OBB::calcNormals()
{
    for (int i = 0; i < vertices.size(); ++i)
    {
        auto edge = vertices[(i + 1) % vertices.size()] - vertices[i];
        auto perp = glm::vec2{ edge.y, -edge.x };
        normals.push_back(glm::normalize(perp));
    }
}

std::pair<float, float> OBB::project(glm::vec2 axis) const
{
    auto projection = glm::dot(vertices[0], axis);
    auto result = std::make_pair(projection, projection);
    for (int i = 1; i < vertices.size(); ++i)
    {
        projection = glm::dot(vertices[i], axis);
        result.first = std::min(result.first, projection);
        result.second = std::max(result.second, projection);
    }
    return result;
}

glm::vec2 OBB::findFurthestPoint(glm::vec2 dir) const
{
    glm::vec2 maxPoint;
    float maxDist = -FLT_MAX;
    for (auto vertex : vertices)
    {
        float dist = glm::dot(vertex, dir);
        if (dist > maxDist)
        {
            maxDist = dist;
            maxPoint = vertex;
        }
    }
    return maxPoint;
}

glm::vec2 OBB::support(const OBB& that, glm::vec2 dir) const
{
    return findFurthestPoint(dir) - that.findFurthestPoint(-dir);
}
