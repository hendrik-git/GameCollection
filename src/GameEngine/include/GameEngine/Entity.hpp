/// @file Entity

#pragma once
#include "Component.hpp"
#include <memory>
#include <string_view>

class Entity
{
  public:
	// shared pointers to the components will come here
	std::shared_ptr<Transform> transform;
	std::shared_ptr<Shape>	   shape;
	std::shared_ptr<Collision> collision;
	std::shared_ptr<Score>	   score;
	std::shared_ptr<Lifespan>  lifespan;
	std::shared_ptr<Input>	   input;
	std::shared_ptr<Mouse>	   mouse;

	void destroy()
	{
		alive_ = false;
	}

	[[nodiscard]] auto id() const -> size_t
	{
		return id_;
	}

	[[nodiscard]] auto tag() const -> std::string_view
	{
		return tag_;
	}

	[[nodiscard]] auto is_alive() const -> bool
	{
		return alive_;
	}

  private:
	// private constructor allows only the EntityManager to create instances
	Entity(std::string_view tag, size_t id) : tag_(tag), id_(id) {}
	friend class EntityManager;

	const size_t		   id_	  = 0;
	const std::string_view tag_	  = "default";
	bool				   alive_ = true;
};