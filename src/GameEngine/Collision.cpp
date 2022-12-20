#include <GameEngine/Collision.hpp>

namespace Engine::Physics
{
	// Two circles collide, if their midpoint distance is less than the sum of their radii. This
	// function uses the squared distance as an optimization instead
	bool CollisionChecker::operator()(sf::CircleShape& first, sf::CircleShape& second)
	{
		//      r1 + r2   >  | pos2 - pos1 |
		// <=>  r1 + r2   >  sqrt( (x2 - x1)^2 + (y2 - y1)^2 )
		// <=>  (r1+r2)^2 > (x2 - x1)^2 + (y2 - y1)^2
		const auto r_dist = second.getRadius() + first.getRadius();
		const auto dx	  = second.getPosition().x - first.getPosition().x;
		const auto dy	  = second.getPosition().y - first.getPosition().y;

		return ((r_dist * r_dist) > (dx * dx) + (dy * dy));
	}

	bool CollisionChecker::operator()(sf::RectangleShape& first, sf::RectangleShape& second)
	{
		return first.getGlobalBounds().findIntersection(second.getGlobalBounds()).has_value();
	}

	bool CollisionChecker::operator()(sf::CircleShape& circle, sf::RectangleShape& rect)
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

	bool CollisionChecker::operator()(sf::RectangleShape& rect, sf::CircleShape& circle)
	{
		return CollisionChecker()(circle, rect);
	}

	bool CollisionChecker::operator()(sf::CircleShape& circle, sf::Vector2f& point)
	{
		// point is a circle without radius
		const auto r_dist = circle.getRadius() + 0.F;
		const auto dx	  = circle.getPosition().x - point.x;
		const auto dy	  = circle.getPosition().y - point.y;
		return ((r_dist * r_dist) > (dx * dx) + (dy * dy));
	}

	//[[nodiscard]] bool operator()(sf::RectangleShape& rect, sf::Vector2f& point)
	//{
	//	return false;
	//}
}  // namespace Engine::Physics