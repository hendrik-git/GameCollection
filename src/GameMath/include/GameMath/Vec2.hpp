
/// @file Vec2.hpp
/// @brief Defines a class representing a two dimensional vector
/// @details ...
/// @see ...
/// @date 2022.11.16
/// @author Hendrik Poettker

#pragma once
#include <cmath>

class Vec2
{
  public:
	float x;
	float y;

	Vec2(float pos_x, float pos_y) : x(pos_x), y(pos_y) {}

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

	[[nodiscard]] auto operator==(Vec2 rhs) const -> bool
	{
		/// @todo epsilon for floating point comparisons
		return (x == rhs.x && y == rhs.y);
	}

	[[nodiscard]] auto operator!=(Vec2 rhs) const -> bool
	{
		/// @todo epsilon for floating point comparisons
		return (x != rhs.x || y != rhs.y);
	}

	[[nodiscard]] auto operator<(Vec2 rhs) const -> bool
	{
		/// @todo epsilon for floating point comparisons
		return (length() < rhs.length());
	}

	// todo C++20 try out operator<=>

	[[nodiscard]] auto length() const -> float
	{
		return std::sqrt(x * x + y * y);
	}

	[[nodiscard]] auto normalize() const -> Vec2
	{
		const auto len{length()};
		return Vec2{x / len, y / len};
	}
};

inline auto get_distance_sq(Vec2 start, Vec2 end) -> float
{
	const auto delta_x = start.x - end.x;
	const auto delta_y = start.y - end.y;
	return delta_x * delta_x + delta_y * delta_y;
}

inline auto get_distance(Vec2 start, Vec2 end) -> float
{
	return std::sqrt(get_distance_sq(start, end));
}

inline auto direction_from_radians(float angle) -> Vec2 
{
	return {std::cos(angle), std::sin(angle)};
}

inline auto direction_from_degree(float angle) -> Vec2 
{
	const auto angle_rad = angle * 3.14159F / 180.F;
	return direction_from_radians(angle_rad);
}