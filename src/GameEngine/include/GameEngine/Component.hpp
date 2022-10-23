/// @file Component

#pragma once
#include <GameMath/Vec2.hpp>
#include <SFML/Graphics.hpp>

struct Transform
{
	Transform(Vec2 pos, Vec2 vel, Vec2 acc, float angle)
		: pos(pos), vel(vel), acc(acc), angle(angle)
	{
	}

	Vec2  pos	= {0.f, 0.f};
	Vec2  vel	= {0.f, 0.f};
	Vec2  acc	= {0.f, 0.f};
	float angle = 0.f;
};

struct ShapeInit
{
	float	  radius	= 10.F;
	size_t	  points	= 32;
	sf::Color fill		= sf::Color::Black;
	sf::Color outline	= sf::Color::White;
	float	  thickness = 2.F;
};

struct Shape
{
	Shape(ShapeInit init) : Shape(init.radius, init.points, init.fill, init.outline, init.thickness)
	{
	}

	Shape(float radius, size_t points, sf::Color fill, sf::Color outline, float thickness)
		: circle(radius, points)
	{
		circle.setFillColor(fill);
		circle.setOutlineColor(outline);
		circle.setOutlineThickness(thickness);
		circle.setOrigin({radius, radius});
	}

	sf::CircleShape circle;
};

struct Collision
{
	Collision(float radius) : radius(radius) {}

	float radius = 0;
};

struct Score
{
	Score(int score) : score(score) {}

	int score = 0;
};

struct Lifespan
{
	Lifespan(int total) : remaining(total), total(total) {}

	int remaining = 0;
	int total	  = 0;
};

struct Input
{
	bool up	   = false;
	bool left  = false;
	bool right = false;
	bool down  = false;
	bool space = false;
};

struct Mouse
{
	bool  lmb = false;
	bool  rmb = false;
	float x = 0.F;
	float y = 0.F;
};