/// @file Collision.hpp
/// @brief Declares CollisionChecker, which is used to check if two shapes collide
/// @details For components that hold a hitbox shape as a component, use std::visit
/// @see Engine::Components::Collision
/// @todo Collision for Box <-> Point and Checker for overlap
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
		/// @details Two circles collide, if their midpoint distance is less than the sum of their
		/// radii. This function uses the squared distance as an optimization instead
		/// \f[ r_1 + r_2 > \left| pos_2 - pos_1 \right| \f]
		/// \f[ \Leftrightarrow r_1 + r_2   >  \sqrt{ (x_2 - x_1)^2 + (y_2 - y_1)^2 } \f]
		/// \f[ \Leftrightarrow (r_1+r_2)^2 > (x_2 - x_1)^2 + (y_2 - y_1)^2 \f]
		/// @param first circle
		/// @param second circle
		/// @return true if they collide
		[[nodiscard]] bool operator()(const sf::CircleShape& first,
									  const sf::CircleShape& second) const;

		/// @brief Checks if two rectangles collide
		/// @note This function uses the intersection detection for rectangles from SFML
		/// @return true when the shapes collide
		[[nodiscard]] bool operator()(const sf::RectangleShape& first,
									  const sf::RectangleShape& second) const;

		/// @brief Checks if a circle and a rectangle collide
		/// @details Determines if a circle and a rectanle collide by checking three distinct cases:
		///	1. The circle center is far from the rectangle center
		/// \f[ dx > \frac{w_{rect}}{2} \text{ or } dy > \frac{h_{rect}}{2} \f]
		/// @return true when the shapes collide
		/// 2. The circle center lies inside the rectangle
		/// \f[ dx <= \frac{w_{rect}}{2} \text{ or } dy <= \frac{h_{rect}}{2} \f]
		/// 3. Edge case for the edges
		[[nodiscard]] bool operator()(const sf::CircleShape&	circle,
									  const sf::RectangleShape& rect) const;

		/// @brief Checks if a circle and a rectangle collide
		/// @return true when the shapes collide
		[[nodiscard]] bool operator()(const sf::RectangleShape& rect,
									  const sf::CircleShape&	circle) const;

		/// @brief Checks if a point lies within a circle
		/// @return true when the shapes collide
		[[nodiscard]] bool operator()(const sf::CircleShape& circle,
									  const sf::Vector2f&	 point) const;
	};

}  // namespace Engine::Physics