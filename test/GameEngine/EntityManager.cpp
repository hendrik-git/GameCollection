/// @file EntityManager.cpp

// see https://catch2.docsforge.com/
#include <GameEngine/EntityManager.hpp>
#include <catch2/catch_all.hpp>

TEST_CASE("EntityManager", "[GameEngine]")
{
	EntityManager manager;
	REQUIRE(manager.get_entities().size() == 0);

	SECTION("Adding entities")
	{
		// Adding an entity does not increase the entity map immediately
		// instead entities are held in a queue and added after the update() call
		manager.add_entity("Player");
		manager.add_entity("Enemy");
		manager.add_entity("Tile");

		// Now three entities should be available
		REQUIRE_NOTHROW(manager.update());
		CHECK(manager.get_entities().size() == 3);
	}

	SECTION("Remove dead entities")
	{
		manager.add_entity("Player");
		manager.add_entity("Enemy");
		manager.add_entity("Tile");
		REQUIRE_NOTHROW(manager.update());

		// schedule all entities to be removed in the next update() call
		for(auto& entity : manager.get_entities())
		{
			entity->destroy();
		}

		// Now no entities should be available
		REQUIRE_NOTHROW(manager.update());
		CHECK(manager.get_entities().size() == 0);
	}
}