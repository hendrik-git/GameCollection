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
#include <format>
#include <iostream>
#include <tuple>
#include <variant>

namespace Engine::Components
{
	/// @brief Base Component interface, determines if a component was added to an entity
	struct BaseComponent
	{
		bool has = false;

		// by adding an implicit conversion to bool, it is no longer necessary to ask for 'has'
		explicit operator bool() const
		{
			return has;
		};
	};

	/// @brief Holds data related to movement, position and angle
	struct Transform : public BaseComponent
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
	struct Shape : public BaseComponent
	{
		explicit Shape(const ShapeInit& init = ShapeInit{})
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
	/// @details This component slightly violates the ECS pattern, by having utility functions
	/// inside a class, that is supposed to hold data only. However the advantage of this deviation
	/// is, that common functionality pertaining to animations does not have to be reimplemented for
	/// each scene.
	struct Drawable
		: public BaseComponent
		, public Animation
	{
		Drawable() = default;

		Drawable(const std::string& name, sf::Texture& texture) : Animation(name, texture) {}


		Drawable(const std::string& name, sf::Texture& texture, size_t frame_count, size_t speed)
			: Animation(name, texture, frame_count, speed)
		{
		}
	};

	using CollShape = std::variant<sf::CircleShape, sf::RectangleShape>;

	/// @brief Holds the shape, that is used to check for collisions with another entity.
	struct Collision : public BaseComponent
	{
		explicit Collision(CollShape hitbox = CollShape{}) : shape(std::move(hitbox)){};

		CollShape shape;
	};

	/// @brief Holds an amount of value attributed to this entity.
	struct Score : public BaseComponent
	{
		explicit Score(int score = 0) : score(score) {}

		int score;
	};

	/// @brief Shows certain doom aproaching
	struct Lifespan : public BaseComponent
	{
		explicit Lifespan(int total = 0) : remaining(total), total(total) {}

		int remaining;
		int total;
	};

	/// @brief This component indicates the entity may use key press input
	struct Input : public BaseComponent
	{
		bool up	   = false;
		bool left  = false;
		bool right = false;
		bool down  = false;
		bool space = false;
	};

	/// @brief This component indicates the entity may use mouse input
	struct Mouse : public BaseComponent
	{
		bool  lmb = false;
		bool  rmb = false;
		float x	  = 0.F;
		float y	  = 0.F;
	};

	/// @brief Holds the amount of damage an entity can withstand
	struct Hitpoints : public BaseComponent
	{
		explicit Hitpoints(int start_hp = 1) : max_hp(start_hp), current_hp(start_hp) {}

		int	 max_hp;
		int	 current_hp;
		bool invulnerable = false;
	};

	/// @brief Hold all components in one container
	using ComponentTuple =
		std::tuple<Transform, Drawable, Shape, Collision, Score, Lifespan, Input, Mouse, Hitpoints>;

}  // namespace Engine::Components