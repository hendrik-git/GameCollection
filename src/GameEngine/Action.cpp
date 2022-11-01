#include <GameEngine/Action.hpp>

Action::Action() {}

Action::Action(std::string name, std::string type) : name_(name), type_(type) {}

auto Action::name() -> std::string&
{
	return name_;
}

auto Action::type() -> std::string&
{
	return type_;
}

auto Action::to_string() -> std::string
{
	return std::string();
}
