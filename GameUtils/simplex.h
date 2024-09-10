#pragma once
#include <array>

#include "cinder/gl/gl.h"

// Source: https://winter.dev/articles/gjk-algorithm
template <typename T, int dim>
class Simplex
{
public:
	Simplex()
		: m_size(0)
	{}

	Simplex& operator=(std::initializer_list<T> list)
	{
		m_size = 0;

		for (T point : list)
			m_points[m_size++] = point;

		return *this;
	}

	void push_front(T point)
	{
		m_points = { point, m_points[0], m_points[1], m_points[2] };
		m_size = std::min(m_size + 1, dim);
	}

	T& operator[](int i) { return m_points[i]; }
	size_t size() const { return m_size; }

	auto begin() const { return m_points.begin(); }
	auto end() const { return m_points.end() - (dim - m_size); }

private:
	std::array<T, 4> m_points;
	int m_size;
};

using Simplex2D = Simplex<glm::vec3, 3>;
using Simplex3D = Simplex<glm::vec3, 4>;
