#pragma once
#include "intersection.h"
#include "simplex.h"

#include "cinder/gl/gl.h"

// Source: https://winter.dev/articles/epa-algorithm
glm::vec2 EPA(std::vector<glm::vec2> polytope, const OBB& shapeA, const OBB& shapeB)
{
    int minIndex = 0;
    float minDistance = FLOAT_INF;
    glm::vec2 minNormal;

    while (minDistance == FLOAT_INF)
    {
        for (int i = 0; i < polytope.size(); ++i)
        {
            int j = (i + 1) % polytope.size();
            
            auto vertexI = polytope[i];
            auto vertexJ = polytope[j];
            auto ij = vertexJ - vertexI;

            auto normal = glm::normalize(glm::vec2(ij.y, -ij.x));
            auto distance = glm::dot(normal, vertexI);

            if (distance < 0)
            {
                distance *= -1;
                normal *= -1;
            }

            if (distance < minDistance)
            {
                minDistance = distance;
                minNormal = normal;
                minIndex = j;
            }
        }

        auto support = shapeA.support(shapeB, minNormal);
        auto suppDistance = glm::dot(minNormal, support);

        if (abs(suppDistance - minDistance) > epsilon)
        {
            minDistance = FLOAT_INF;
            polytope.insert(polytope.begin() + minIndex, support);
        }
    }

    return minNormal * (minDistance + epsilon);
}
