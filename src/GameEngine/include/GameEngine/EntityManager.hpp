/// @file EntityManager

#pragma once
#include "Entity.hpp"
#include <map>
#include <memory>
#include <vector>

using entity_vec = std::vector<std::shared_ptr<Entity>>;
using entity_map = std::map<std::string_view, entity_vec>;

class EntityManager
{
  public:
	auto add_entity(std::string_view tag) -> std::shared_ptr<Entity>
	{
		static size_t nr{0};
		auto		  e = std::shared_ptr<Entity>(new Entity(tag, ++nr));
		to_add_.push_back(e);
		return e;
	}

	auto get_entities() -> entity_vec&
	{
		return entities_;
	}

	auto get_entities(std::string_view tag) -> entity_vec&
	{
		return tag_map_.at(tag);
	}

	void update()
	{
		// remove dead entries from vector
		entities_.erase(std::remove_if(entities_.begin(),
									   entities_.end(),
									   [&](const Entity ent) -> bool { return !ent.alive_; }),
						entities_.end());

		// tag_map_ erase as well

		//
		for(auto entity : to_add_)
		{
			entities_.push_back(entity);
			tag_map_[entity->tag_].push_back(entity);
		}
	}
	void destroy() {}

  private:
	entity_vec entities_;
	entity_map tag_map_;
	entity_vec to_add_;
	size_t	   total_entities_;
};