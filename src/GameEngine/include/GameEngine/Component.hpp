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

	Vec2   pos	 = {0, 0};
	Vec2   vel	 = {0, 0};
	Vec2   acc	 = {0, 0};
	double angle = 0;
};

struct Shape
{
	Shape(float radius, int points, sf::Color& fill, sf::Color& outline, float thickness)
		: circle(radius, points)
	{
		circle.setFillColor(fill);
		circle.setOutlineColor(fill);
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