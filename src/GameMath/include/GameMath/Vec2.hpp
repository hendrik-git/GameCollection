/// @file vec2

#pragma once
#include <cmath>

class Vec2
{
  public:
	float x;
	float y;

	Vec2(float x, float y) : x(x), y(y) {}

	[[nodiscard]] auto operator+(Vec2 rhs) const
	{
		return Vec2{x + rhs.x, y + rhs.y};
	}

	[[nodiscard]] auto operator-(Vec2 rhs) const
	{
		return Vec2{x - rhs.x, y - rhs.y};
	}

	[[nodiscard]] auto operator*(float scale) const
	{
		return Vec2{x * scale, y * scale};
	}

	[[nodiscard]] auto operator/(float scale) const
	{
		return Vec2{x / scale, y / scale};
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

	[[nodiscard]] auto operator==(Vec2 rhs) -> bool
	{
		/// @todo epsilon for floating point comparisons
		return (x == rhs.x && y == rhs.y);
	}

	[[nodiscard]] auto operator!=(Vec2 rhs) -> bool
	{
		/// @todo epsilon for floating point comparisons
		return (x != rhs.x || y != rhs.y);
	}

	[[nodiscard]] auto operator<(Vec2 rhs) -> bool
	{
		/// @todo epsilon for floating point comparisons
		return (length() < rhs.length());
	}

	// todo C++20 try out operator<=>

	[[nodiscard]] auto length() -> float
	{
		return std::sqrt(x * x + y * y);
	}

	[[nodiscard]] auto normalize() -> Vec2 const
	{
		const auto len{length()};
		return Vec2{x / len, y / len};
	}
};

inline float get_distance_sq(Vec2 a, Vec2 b)
{
	auto delta_x = a.x - b.x;
	auto delta_y = a.y - b.y;
	return delta_x * delta_x + delta_y * delta_y;
}

inline float get_distance(Vec2 a, Vec2 b)
{
	return std::sqrt(get_distance_sq(a, b));
}

inline Vec2 direction_from_radians(float angle) 
{
	return Vec2(std::cos(angle), std::sin(angle));
}

inline Vec2 direction_from_degree(float angle) 
{
	const auto angle_rad = angle * 3.14159F / 180.F;
	return direction_from_radians(angle_rad);
}