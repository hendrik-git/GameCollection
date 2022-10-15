/// @file EntityManager

#pragma once
#include "Entity.hpp"
#include <map>
#include <memory>
#include <vector>

using entity_vec = std::vector<std::shared_ptr<Entity>>;
using entity_map = std::map<std::string_view, std::shared_ptr<Entity>>;

class EntityManager
{
  public:
	auto add_entity(std::string_view tag) -> std::shared_ptr<Entity> 
	{
		auto e = std::shared_ptr<Entity>(new Entity(tag));
	}
	auto get_entities() -> entity_vec&;
	auto get_entities(std::string_view tag) -> entity_vec&;
	void update();
	void destroy();

  private:
	entity_vec entities_;
	entity_vec to_add_;
	size_t	   total_entities_;
};