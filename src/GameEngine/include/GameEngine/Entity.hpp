/// @file Entity.hpp
/// @brief ...
/// @details ...
/// @see ...
/// @date 2022.11.16
/// @author Hendrik Poettker

#pragma once
#include "Component.hpp"
#include <memory>
#include <string_view>
#include <tuple>

class EntityManager;

class Entity
{
  public:
	ComponentTuple components;

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

	template<typename T>
	[[nodiscard]] bool has_component() const
	{
		return std::get<T>(components).has;
	}

	template<typename T, typename... TArgs>
	T& add_component(TArgs&&... args)
	{
		auto& component = get_component<T>();
		component		= T(std::forward<TArgs>(args)...);
		component.has	= true;
		return component;
	}

	template<typename T>
	void remove_component()
	{
		get_component<T>() = T{};
	}

	template<typename T>
	[[nodiscard]] T& get_component()
	{
		return std::get<T>(components);
	}

	template<typename T>
	[[nodiscard]] const T& get_component() const
	{
		return std::get<T>(components);
	}

  private:
	// private constructor allows only the EntityManager to create instances
	Entity(std::string_view tag, size_t id) : tag_(tag), id_(id) {}
	friend class EntityManager;

	const std::string_view tag_	  = "default";
	const size_t		   id_	  = 0;
	bool				   alive_ = true;
};