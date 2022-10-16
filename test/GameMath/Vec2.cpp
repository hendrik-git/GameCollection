/// @file Unit tests for the Vec2 class

// see https://catch2.docsforge.com/
#include <GameMath/Vec2.hpp>
#include <catch2/catch_all.hpp>

TEST_CASE("Math", "[Vec2]")
{
	Vec2 test{0.1f, 0.2f};
	REQUIRE(test.x == 0.1f);
	REQUIRE(test.y == 0.2f);
}