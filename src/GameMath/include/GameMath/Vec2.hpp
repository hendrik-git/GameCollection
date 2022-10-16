/// @file vec2

#pragma once
#include <cmath>

class Vec2
{
  public:
	float x;
	float y;

	Vec2(float x, float y) : x(x), y(y) {}

	auto operator+(Vec2 rhs) const
	{
		return Vec2{x + rhs.x, y + rhs.y};
	}

	auto operator-(Vec2 rhs) const
	{
		return Vec2{x - rhs.x, y - rhs.y};
	}

	void operator+=(Vec2 rhs)
	{
		x += rhs.x;
		y += rhs.y;
	}

	void operator-=(Vec2 rhs)
	{
		x -= rhs.x;
		y -= rhs.y;
	}

	void operator*=(float scale)
	{
		x *= scale;
		y *= scale;
	}

	void operator/=(float scale)
	{
		x /= scale;
		y /= scale;
	}

	auto normalize() -> Vec2 const
	{
		const auto length{std::sqrt(x * x + y * y)};
		return Vec2{x / length, y / length};
	}
};