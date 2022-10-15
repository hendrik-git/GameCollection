/// @file Entity

#pragma once
#include <string_view>

class Entity
{
  public:
	// shared pointers to the components will come here

  private:
	// private constructor allows only the EntityManager to create instances
	Entity(std::string_view tag, size_t id) : tag_(tag), id_(id) {}
	friend class EntityManager;

	const size_t		   id_	  = 0;
	const std::string_view tag_	  = "default";
	bool				   alive_ = true;
};