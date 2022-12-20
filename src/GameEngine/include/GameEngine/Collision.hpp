/// @file Collision.hpp
/// @brief Declares CollisionChecker, which is used to check if two shapes collide
/// @details For components that hold a hitbox shape as a component, use std::visit
/// @see Engine::Components::Collision
/// @date 2022.12.20
/// @author Hendrik Poettker

#pragma once
#include <SFML/Graphics.hpp>

namespace Engine::Physics
{
	/// @brief Visitor class for collision detection of shapes
	/// @detail An entity with a collision component stores a hitbox shape in a std::variant. To
	/// detect collision with another entity, the shape-variants are used with the visitor
	/// pattern as arguments to this class
	struct CollisionChecker
	{
		/// @brief Checks if two circles collide
		/// @return true when the shapes collide
		[[nodiscard]] bool operator()(sf::CircleShape& first, sf::CircleShape& second);

		/// @brief Checks if two rectangles collide
		/// @note This function uses the intersection detection for rectangles from SFML
		/// @return true when the shapes collide
		[[nodiscard]] bool operator()(sf::RectangleShape& first, sf::RectangleShape& second);

		/// @brief Checks if a circle and a rectangle collide
		/// @return true when the shapes collide
		[[nodiscard]] bool operator()(sf::CircleShape& circle, sf::RectangleShape& rect);

		/// @brief Checks if a circle and a rectangle collide
		/// @return true when the shapes collide
		[[nodiscard]] bool operator()(sf::RectangleShape& rect, sf::CircleShape& circle);

		/// @brief Checks if a point lies within a circle
		/// @return true when the shapes collide
		[[nodiscard]] bool operator()(sf::CircleShape& circle, sf::Vector2f& point);
	};

}  // namespace Engine::Physics