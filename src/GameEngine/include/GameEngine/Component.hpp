/// @file Component

#pragma once
#include <GameMath/Vec2.hpp>
#include <SFML/Graphics.hpp>
#include <tuple>

struct Component
{
	bool has = false;
};

struct Transform : public Component
{
	Transform(Vec2 pos = {0.f, 0.f}, Vec2 vel = {0.f, 0.f}, float angle = 0.f)
		: pos(pos), prev_pos(pos), vel(vel), angle(angle)
	{
	}

	Vec2  pos;
	Vec2  prev_pos;
	Vec2  vel;
	Vec2  scale = {1.f, 1.f};
	float angle;
};

struct ShapeInit
{
	float	  radius	= 10.F;
	size_t	  points	= 32;
	sf::Color fill		= sf::Color::Black;
	sf::Color outline	= sf::Color::White;
	float	  thickness = 2.F;
};

struct Shape : public Component
{
	Shape(ShapeInit init = ShapeInit{})
		: Shape(init.radius, init.points, init.fill, init.outline, init.thickness)
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

struct Collision : public Component
{
	Collision(float radius = 0.f) : radius(radius) {}

	float radius;
};

struct Score : public Component
{
	Score(int score = 0) : score(score) {}

	int score;
};

struct Lifespan : public Component
{
	Lifespan(int total = 0) : remaining(total), total(total) {}

	int remaining;
	int total;
};

struct Input : public Component
{
	bool up	   = false;
	bool left  = false;
	bool right = false;
	bool down  = false;
	bool space = false;
};

struct Mouse : public Component
{
	bool  lmb = false;
	bool  rmb = false;
	float x	  = 0.F;
	float y	  = 0.F;
};


using ComponentTuple = std::tuple<Transform, Shape, Collision, Score, Lifespan, Input, Mouse>;