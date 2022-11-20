/// @file Action.hpp
/// @brief ...
/// @details ...
/// @see ...
/// @date 2022.11.16
/// @author Hendrik Poettker

#pragma once
#include <string>

class Action
{
  public:
	Action();
	Action(std::string name, std::string type);

	auto name() -> std::string&;
	auto type() -> std::string&;
	auto name() const -> const std::string&;
	auto type() const -> const std::string&;
	auto to_string() -> std::string;

  private:
	std::string name_;
	std::string type_;
};