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
	void init() {}

	void update()
	{
		// remove dead entries from vector
		entities_.erase(std::remove_if(entities_.begin(),
									   entities_.end(),
									   [&](const std::shared_ptr<Entity> ent) -> bool
									   { return !ent->alive_; }),
						entities_.end());

		// tag_map_ erase as well
		for(auto& [key, value] : tag_map_)
		{
			value.erase(std::remove_if(value.begin(),
									   value.end(),
									   [&](const std::shared_ptr<Entity> ent) -> bool
									   { return !ent->alive_; }),
						value.end());
		}

		//
		for(auto entity : to_add_)
		{
			entities_.push_back(entity);
			tag_map_[entity->tag_].push_back(entity);
		}
		to_add_.clear();
	}

	auto add_entity(std::string_view tag) -> std::shared_ptr<Entity>
	{
		auto e = std::shared_ptr<Entity>(new Entity(tag, ++total_entities_));
		to_add_.push_back(e);
		return e;
	}

	[[nodiscard]] auto get_entities() -> entity_vec&
	{
		return entities_;
	}

	[[nodiscard]] auto get_entities(std::string_view tag) -> entity_vec&
	{
		// ensure that the requested key is a valid target in the map
		if(tag_map_.find(tag) == tag_map_.end())
		{
			tag_map_[tag] = entity_vec{};
		}
		return tag_map_[tag];
	}

  private:
	entity_vec entities_;
	entity_map tag_map_;
	entity_vec to_add_;
	size_t	   total_entities_ = 0;
};