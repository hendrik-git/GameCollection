#include <GameEngine/Collision.hpp>

namespace Engine::Physics
{

	bool CollisionChecker::operator()(const sf::CircleShape& first,
									  const sf::CircleShape& second) const
	{
		const auto r_dist = second.getRadius() + first.getRadius();
		const auto dx	  = second.getPosition().x - first.getPosition().x;
		const auto dy	  = second.getPosition().y - first.getPosition().y;

		return ((r_dist * r_dist) > (dx * dx) + (dy * dy));
	}

	bool CollisionChecker::operator()(const sf::RectangleShape& first,
									  const sf::RectangleShape& second) const
	{
		return first.getGlobalBounds().findIntersection(second.getGlobalBounds()).has_value();
	}

	bool CollisionChecker::operator()(const sf::CircleShape&	circle,
									  const sf::RectangleShape& rect) const
	{
		const auto dx = std::abs(rect.getPosition().x - circle.getPosition().x);
		const auto dy = std::abs(rect.getPosition().y - circle.getPosition().y);

		// check if the circle lies far outside the rectangle bounds
		if(dx > (rect.getSize().x / 2.F + circle.getRadius()) ||
		   dy > (rect.getSize().y / 2.F + circle.getRadius()))
		{
			return false;
		}

		// check if the circle lies definitely within the rectangle bounds
		if(dx <= (rect.getSize().x / 2.F) || dy <= (rect.getSize().y / 2.F))
		{
			return true;
		}

		// finally the edge case when the circle is close to the edge
		const auto r_dist = circle.getRadius() * circle.getRadius();
		const auto edge_x = (dx - rect.getSize().x / 2.F);
		const auto edge_y = (dy - rect.getSize().y / 2.F);
		const auto dist	  = edge_x * edge_x + edge_y * edge_y;
		return (dist <= r_dist);
	}

	bool CollisionChecker::operator()(const sf::RectangleShape& rect,
									  const sf::CircleShape&	circle) const
	{
		return CollisionChecker()(circle, rect);
	}

	bool CollisionChecker::operator()(const sf::CircleShape& circle,
									  const sf::Vector2f&	 point) const
	{
		// point is a circle without radius
		const auto r_dist = circle.getRadius() + 0.F;
		const auto dx	  = circle.getPosition().x - point.x;
		const auto dy	  = circle.getPosition().y - point.y;
		return ((r_dist * r_dist) > (dx * dx) + (dy * dy));
	}
}  // namespace Engine::Physics