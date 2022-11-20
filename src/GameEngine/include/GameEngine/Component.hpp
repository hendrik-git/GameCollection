/// @file Component.hpp
/// @brief Defines the data holding components of entities
/// @details Components inherit from a base class, that holds a value if the component is present
/// @see Entity Component System
/// @date 2022.11.16
/// @author Hendrik Poettker

#pragma once
#include <GameEngine/Animation.hpp>
#include <GameMath/Vec2.hpp>
#include <SFML/Graphics.hpp>
#include <tuple>

/// @brief Base Component interface, determines if a component was added to an entity
struct Component
{
	bool has = false;
};

/// @brief Holds data related to movement, position and angle
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

/// @brief Initial component for visualization, superceeded by Drawable
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

/// @brief Holds data related to animation and texture
/// @details This component slightly violates the ECS pattern, by having utility functions inside a
/// class, that is supposed to hold data only. However the advantage of this deviation is, that
/// common functionality pertaining to animations does not have to be reimplemented for each scene.
struct Drawable
	: public Component
	, public Animation
{
	Drawable() {}

	Drawable(std::string name, sf::Texture& texture) : Animation(name, texture) {}


	Drawable(std::string name, sf::Texture& texture, size_t frame_count, size_t speed)
		: Animation(name, texture, frame_count, speed)
	{
	}
};

/// @brief Holds the shape, that is used to check for collisions with another entity.
struct Collision : public Component
{
	Collision(float radius = 0.f) : radius(radius) {}

	float radius;
};

/// @brief Holds an amount of value attributed to this entity.
struct Score : public Component
{
	Score(int score = 0) : score(score) {}

	int score;
};

/// @brief Shows certain doom aproaching
struct Lifespan : public Component
{
	Lifespan(int total = 0) : remaining(total), total(total) {}

	int remaining;
	int total;
};

/// @brief This component indicates the entity may use key press input
struct Input : public Component
{
	bool up	   = false;
	bool left  = false;
	bool right = false;
	bool down  = false;
	bool space = false;
};

/// @brief This component indicates the entity may use mouse input
struct Mouse : public Component
{
	bool  lmb = false;
	bool  rmb = false;
	float x	  = 0.F;
	float y	  = 0.F;
};

/// @brief Holds the amount of damage an entity can withstand
struct Hitpoints : public Component
{
	Hitpoints(int start_hp = 1) : max_hp(start_hp), current_hp(start_hp), invulnerable(false) {}

	int	 max_hp;
	int	 current_hp;
	bool invulnerable;
};

/// @brief Hold all components in one container
using ComponentTuple =
	std::tuple<Transform, Drawable, Shape, Collision, Score, Lifespan, Input, Mouse, Hitpoints>;