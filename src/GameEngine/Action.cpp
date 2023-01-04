#include <GameEngine/Action.hpp>

Engine::Systems::Action::Action(std::string name, ActionType type, MousePos mouse)
	: name_(std::move(name)), type_(type), mouse_(std::move(mouse))
{
}

auto Engine::Systems::Action::name() const -> const std::string&
{
	return name_;
}

auto Engine::Systems::Action::type() const -> ActionType
{
	return type_;
}