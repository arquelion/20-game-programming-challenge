#pragma once
#include "intersection.h"
#include "simplex.h"

#include "cinder/gl/gl.h"

// Source: https://winter.dev/articles/gjk-algorithm
bool isSameDirection(const glm::vec2& direction, const glm::vec2& ao)
{
	return dot(direction, ao) > 0;
}

bool Line(Simplex2D& points, glm::vec2& direction)
{
	glm::vec2 a = points[0];
	glm::vec2 b = points[1];

	glm::vec2 ab = b - a;
	glm::vec2 ao = -a;

	if (isSameDirection(ab, ao))
	{
		direction = ao;
	}
	else
	{
		points = { a };
		direction = ao;
	}

	return false;
}

bool Triangle(Simplex2D& points, glm::vec2& direction)
{
	glm::vec2 a = points[0];
	glm::vec2 b = points[1];
	glm::vec2 c = points[2];

	glm::vec2 ab = b - a;
	glm::vec2 ac = c - a;
	glm::vec2 ao = -a;

	glm::vec2 abc = glm::vec2(-ac.y, ac.x);
	if (isSameDirection(abc, ab))
	{
		abc *= -1;
	}

	if (isSameDirection(abc, ao))
	{
		if (isSameDirection(ac, ao))
		{
			points = { a, c };
			direction = ao;
		}
		else
		{
			return Line(points = { a, b }, direction);
		}
	}
	else
	{
		if (!isSameDirection(ac, ao))
		{
			return Line(points = { a, b }, direction);
		}
		return true;
	}

	return false;
}

bool NextSimplex2D(Simplex2D& points, glm::vec2& direction)
{
	switch (points.size()) {
	case 2: return Line(points, direction);
	case 3: return Triangle(points, direction);
	}

	// never should be here
	return false;
}

bool GJK(const OBB& colliderA, const OBB& colliderB)
{
	// Get initial support point in any direction
	auto support = colliderA.support(colliderB, glm::vec2(1, 0));

	// Simplex2D is an array of points, max count is 4
	Simplex2D points;
	points.push_front(support);

	// New direction is towards the origin
	auto direction = -support;

	while (true) {
		support = colliderA.support(colliderB, direction);

		if (dot(support, direction) <= 0) {
			return false; // no collision
		}

		points.push_front(support);

		if (NextSimplex2D(points, direction)) {
			return true;
		}
	}
}
