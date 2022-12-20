/// @file Collision.cpp
/// @detail Unit tests for the CollisionChecker class, that returns true, if two shapes in a variant
/// collide. The class makes use of the visitor-pattern to use the correct overload for the given
/// type in the variant.

// see https://catch2.docsforge.com/
#include <GameEngine/Collision.hpp>
#include <GameEngine/Component.hpp>
#include <catch2/catch_all.hpp>

TEST_CASE("Component Collision", "[GameEngine][Physics]")
{
	using namespace Engine::Physics;
	using namespace Engine::Components;

	// --------------------------------------------------------------------------------------------
	SECTION("Circle <-> Circle")
	{
		// create two circles with a radius of 10 pixels
		auto sh1 = sf::CircleShape(10);
		auto sh2 = sf::CircleShape(10);

		// by default, the origin is the top left corner
		// set the origin to the center of the circle
		sh1.setOrigin({5.F, 5.F});
		sh2.setOrigin({5.F, 5.F});


		SECTION("Same position")
		{
			auto circle = v2::CollShape(sh1);
			CHECK(std::get<sf::CircleShape>(circle).getRadius() == 10.F);
			CHECK(std::get<sf::CircleShape>(circle).getPosition() == sf::Vector2f(0.F, 0.F));
			REQUIRE(std::visit(CollisionChecker{}, circle, circle));
		}

		SECTION("Different position with overlap")
		{
			{
				sh1.setPosition({0.F, 0.F});
				sh2.setPosition({15.F, 0.F});
				auto circle1 = v2::CollShape(sh1);
				auto circle2 = v2::CollShape(sh2);
				CHECK(std::get<sf::CircleShape>(circle1).getRadius() == 10.F);
				CHECK(std::get<sf::CircleShape>(circle2).getRadius() == 10.F);
				CHECK(std::get<sf::CircleShape>(circle1).getPosition() == sf::Vector2f(0.F, 0.F));
				CHECK(std::get<sf::CircleShape>(circle2).getPosition() == sf::Vector2f(15.F, 0.F));
				REQUIRE(std::visit(CollisionChecker{}, circle1, circle2));
			}
			{
				sh1.setPosition({0.F, 0.F});
				sh2.setPosition({0.F, -15.F});
				auto circle1 = v2::CollShape(sh1);
				auto circle2 = v2::CollShape(sh2);
				CHECK(std::get<sf::CircleShape>(circle1).getRadius() == 10.F);
				CHECK(std::get<sf::CircleShape>(circle2).getRadius() == 10.F);
				CHECK(std::get<sf::CircleShape>(circle1).getPosition() == sf::Vector2f(0.F, 0.F));
				CHECK(std::get<sf::CircleShape>(circle2).getPosition() == sf::Vector2f(0.F, -15.F));
				REQUIRE(std::visit(CollisionChecker{}, circle1, circle2));
			}
			{
				sh1.setPosition({0.F, 0.F});
				sh2.setPosition({5.F, 5.F});
				auto circle1 = v2::CollShape(sh1);
				auto circle2 = v2::CollShape(sh2);
				CHECK(std::get<sf::CircleShape>(circle1).getRadius() == 10.F);
				CHECK(std::get<sf::CircleShape>(circle2).getRadius() == 10.F);
				CHECK(std::get<sf::CircleShape>(circle1).getPosition() == sf::Vector2f(0.F, 0.F));
				CHECK(std::get<sf::CircleShape>(circle2).getPosition() == sf::Vector2f(5.F, 5.F));
				REQUIRE(std::visit(CollisionChecker{}, circle1, circle2));
			}
		}

		SECTION("Different position without overlap")
		{
			sh1.setPosition({0.F, 0.F});
			sh2.setPosition({125.F, 125.F});
			auto circle1 = v2::CollShape(sh1);
			auto circle2 = v2::CollShape(sh2);
			CHECK(std::get<sf::CircleShape>(circle1).getRadius() == 10.F);
			CHECK(std::get<sf::CircleShape>(circle2).getRadius() == 10.F);
			CHECK(std::get<sf::CircleShape>(circle1).getPosition() == sf::Vector2f(0.F, 0.F));
			CHECK(std::get<sf::CircleShape>(circle2).getPosition() == sf::Vector2f(125.F, 125.F));
			REQUIRE(std::visit(CollisionChecker{}, circle1, circle2) == false);
		}
	}

	// --------------------------------------------------------------------------------------------
	SECTION("Rectangle <-> Rectangle")
	{
		auto sh1 = sf::RectangleShape({10.F, 10.F});
		auto sh2 = sf::RectangleShape({20.F, 5.F});
		sh1.setOrigin({5.F, 5.F});
		sh2.setOrigin({10.F, 2.5F});

		SECTION("Same position")
		{
			sh1.setPosition({0.F, 0.F});
			auto box = v2::CollShape(sh1);
			CHECK(std::get<sf::RectangleShape>(box).getSize() == sf::Vector2f{10.F, 10.F});
			CHECK(std::get<sf::RectangleShape>(box).getPosition() == sf::Vector2f(0.F, 0.F));
			REQUIRE(std::visit(CollisionChecker{}, box, box));
		}

		SECTION("Different position with overlap")
		{
			sh1.setPosition({0.F, 0.F});
			sh2.setPosition({5.F, 5.F});
			auto box1 = v2::CollShape(sh1);
			auto box2 = v2::CollShape(sh2);
			CHECK(std::get<sf::RectangleShape>(box1).getSize() == sf::Vector2f{10.F, 10.F});
			CHECK(std::get<sf::RectangleShape>(box2).getSize() == sf::Vector2f{20.F, 5.F});
			CHECK(std::get<sf::RectangleShape>(box1).getPosition() == sf::Vector2f(0.F, 0.F));
			CHECK(std::get<sf::RectangleShape>(box2).getPosition() == sf::Vector2f(5.F, 5.F));
			REQUIRE(std::visit(CollisionChecker{}, box1, box2));
		}

		SECTION("Different position without overlap")
		{
			sh1.setPosition({0.F, 0.F});
			sh2.setPosition({125.F, 125.F});
			auto box1 = v2::CollShape(sh1);
			auto box2 = v2::CollShape(sh2);
			CHECK(std::get<sf::RectangleShape>(box1).getSize() == sf::Vector2f{10.F, 10.F});
			CHECK(std::get<sf::RectangleShape>(box2).getSize() == sf::Vector2f{20.F, 5.F});
			CHECK(std::get<sf::RectangleShape>(box1).getPosition() == sf::Vector2f(0.F, 0.F));
			CHECK(std::get<sf::RectangleShape>(box2).getPosition() == sf::Vector2f(125.F, 125.F));
			REQUIRE(std::visit(CollisionChecker{}, box1, box2) == false);
		}
	}

	// --------------------------------------------------------------------------------------------
	SECTION("Rectangle <-> Circle")
	{
		auto sh1 = sf::RectangleShape({10.F, 10.F});
		auto sh2 = sf::CircleShape(10.F);
		sh1.setOrigin({5.F, 5.F});
		sh2.setOrigin({5.F, 5.F});

		SECTION("Same position")
		{
			sh1.setPosition({0.F, 0.F});
			sh2.setPosition({0.F, 0.F});
			auto box	= v2::CollShape(sh1);
			auto circle = v2::CollShape(sh2);
			CHECK(std::get<sf::RectangleShape>(box).getSize() == sf::Vector2f{10.F, 10.F});
			CHECK(std::get<sf::RectangleShape>(box).getPosition() == sf::Vector2f(0.F, 0.F));
			CHECK(std::get<sf::CircleShape>(circle).getPosition() == sf::Vector2f(0.F, 0.F));
			CHECK(std::get<sf::CircleShape>(circle).getRadius() == 10.F);
			REQUIRE(std::visit(CollisionChecker{}, box, circle));
		}


		SECTION("Different position with overlap")
		{
			sh1.setPosition({0.F, 0.F});
			sh2.setPosition({12.F, 12.F});
			auto box	= v2::CollShape(sh1);
			auto circle = v2::CollShape(sh2);
			CHECK(std::get<sf::RectangleShape>(box).getSize() == sf::Vector2f{10.F, 10.F});
			CHECK(std::get<sf::RectangleShape>(box).getPosition() == sf::Vector2f(0.F, 0.F));
			CHECK(std::get<sf::CircleShape>(circle).getPosition() == sf::Vector2f(12.F, 12.F));
			CHECK(std::get<sf::CircleShape>(circle).getRadius() == 10.F);
			REQUIRE(std::visit(CollisionChecker{}, box, circle));
		}


		SECTION("Different position without overlap")
		{
			sh1.setPosition({0.F, 0.F});
			sh2.setPosition({125.F, 125.F});
			auto box	= v2::CollShape(sh1);
			auto circle = v2::CollShape(sh2);
			CHECK(std::get<sf::RectangleShape>(box).getSize() == sf::Vector2f{10.F, 10.F});
			CHECK(std::get<sf::RectangleShape>(box).getPosition() == sf::Vector2f(0.F, 0.F));
			CHECK(std::get<sf::CircleShape>(circle).getPosition() == sf::Vector2f(125.F, 125.F));
			CHECK(std::get<sf::CircleShape>(circle).getRadius() == 10.F);
			REQUIRE(std::visit(CollisionChecker{}, box, circle) == false);
		}
	}

	// --------------------------------------------------------------------------------------------
	SECTION("Circle <-> Rectangle")
	{
		auto sh1 = sf::RectangleShape({10.F, 10.F});
		auto sh2 = sf::CircleShape(10.F);
		sh1.setOrigin({5.F, 5.F});
		sh2.setOrigin({5.F, 5.F});

		SECTION("Same position")
		{
			sh1.setPosition({0.F, 0.F});
			sh2.setPosition({0.F, 0.F});
			auto box	= v2::CollShape(sh1);
			auto circle = v2::CollShape(sh2);
			CHECK(std::get<sf::RectangleShape>(box).getSize() == sf::Vector2f{10.F, 10.F});
			CHECK(std::get<sf::RectangleShape>(box).getPosition() == sf::Vector2f(0.F, 0.F));
			CHECK(std::get<sf::CircleShape>(circle).getPosition() == sf::Vector2f(0.F, 0.F));
			CHECK(std::get<sf::CircleShape>(circle).getRadius() == 10.F);
			REQUIRE(std::visit(CollisionChecker{}, circle, box));
		}


		SECTION("Different position with overlap")
		{
			sh1.setPosition({0.F, 0.F});
			sh2.setPosition({12.F, 12.F});
			auto box	= v2::CollShape(sh1);
			auto circle = v2::CollShape(sh2);
			CHECK(std::get<sf::RectangleShape>(box).getSize() == sf::Vector2f{10.F, 10.F});
			CHECK(std::get<sf::RectangleShape>(box).getPosition() == sf::Vector2f(0.F, 0.F));
			CHECK(std::get<sf::CircleShape>(circle).getPosition() == sf::Vector2f(12.F, 12.F));
			CHECK(std::get<sf::CircleShape>(circle).getRadius() == 10.F);
			REQUIRE(std::visit(CollisionChecker{}, circle, box));
		}

		SECTION("Different position without overlap")
		{
			sh1.setPosition({0.F, 0.F});
			sh2.setPosition({125.F, 125.F});
			auto box	= v2::CollShape(sh1);
			auto circle = v2::CollShape(sh2);
			CHECK(std::get<sf::RectangleShape>(box).getSize() == sf::Vector2f{10.F, 10.F});
			CHECK(std::get<sf::RectangleShape>(box).getPosition() == sf::Vector2f(0.F, 0.F));
			CHECK(std::get<sf::CircleShape>(circle).getPosition() == sf::Vector2f(125.F, 125.F));
			CHECK(std::get<sf::CircleShape>(circle).getRadius() == 10.F);
			REQUIRE(std::visit(CollisionChecker{}, circle, box) == false);
		}
	}
}