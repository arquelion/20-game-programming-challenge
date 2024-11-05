#pragma once
#include "intersection.h"
#include "simplex.h"

#include "cinder/gl/gl.h"

// Source: https://winter.dev/articles/gjk-algorithm
bool isSameDirection(const glm::vec3& direction, const glm::vec3& ao)
{
	return glm::dot(direction, ao) > 0;
}

bool Line(Simplex2D& points, glm::vec3& direction)
{
	glm::vec3 a = points[0];
	glm::vec3 b = points[1];

	glm::vec3 ab = b - a;
	glm::vec3 ao = -a;

	if (isSameDirection(ab, ao))
	{
		direction = glm::cross(glm::cross(ab, ao), ab);
		/*direction = {ab.y, -ab.x};
		if (!isSameDirection(direction, ao))
		{
			direction *= -1;
		}*/
	}
	else
	{
		points = { a };
		direction = ao;
	}

	return false;
}

bool Triangle(Simplex2D& points, glm::vec3& direction)
{
	glm::vec3 a = points[0];
	glm::vec3 b = points[1];
	glm::vec3 c = points[2];

	glm::vec3 ab = b - a;
	glm::vec3 ac = c - a;
	glm::vec3 ao = -a;

	glm::vec3 abc = glm::cross(ab, ac);
	//glm::vec3 acPerp = glm::vec3(-ac.y, ac.x);
	/*if (isSameDirection(acPerp, ab))
	{
		acPerp *= -1;
	}*/

	if (isSameDirection(glm::cross(abc, ac), ao))
	{
		if (isSameDirection(ac, ao))
		{
			points = { a, c };
			direction = glm::cross(glm::cross(ac, ao), ac);
		}
		else
		{
			return Line(points = { a, b }, direction);
		}
	}
	else
	{
		if (isSameDirection(glm::cross(ab, abc), ao))
		{
			return Line(points = { a, b }, direction);
		}
		else
		{
			return true;
			/*if (isSameDirection(abc, ao))
			{
				direction = abc;
			}
			else
			{
				points = { a, c, b };
				direction = -abc;
			}*/
		}
	}

	return false;
}

bool NextSimplex2D(Simplex2D& points, glm::vec3& direction)
{
	switch (points.size()) {
	case 2: return Line(points, direction);
	case 3: return Triangle(points, direction);
	}

	// never should be here
	return false;
}

std::optional<Simplex2D> GJK(const OBB& colliderA, const OBB& colliderB)
{
	// Get initial support point in any direction
	glm::vec3 support = { colliderA.support(colliderB, glm::vec2(1, 0)), 0 };

	// Simplex2D is an array of points, max count is 4
	Simplex2D points;
	points.push_front(support);

	// New direction is towards the origin
	glm::vec3 direction = -support;

	while (true) {
		support = { colliderA.support(colliderB, direction), 0 };

		if (dot(support, direction) - epsilon <= 0) {
			return std::nullopt; // no collision
		}

		points.push_front(support);

		if (NextSimplex2D(points, direction)) {
			return std::make_optional(points);
		}
	}
}
