/// @file Action.hpp
/// @brief ...
/// @details ...
/// @see ...
/// @date 2022.11.16
/// @author Hendrik Poettker

#pragma once
#include <optional>
#include <string>

namespace Engine::Systems
{
	enum class ActionType
	{
		Impulse,
		Start,
		End
	};

	using MousePos = std::optional<std::pair<float, float>>;

	/// @brief Abstracts an Action (e.g. Keypress, Mouse)
	/// @detail The GameEngine listens for user input. Any input will be looked up in the current
	/// scene. If the input is registered, an action will be generated for the current scene. Else
	/// the input will be ignored. The action carries the type of the input by name and an
	/// additional flag, whether the input has started or stopped. The abstraction of the input
	/// allows for replay systems and AI controlled input.
	/// @todo Mouse input with additional data (mouse position)
	class Action
	{
	  public:
		Action() = default;
		Action(std::string name, ActionType type, MousePos mouse = MousePos{});

		/// @brief The name is used to check if the action is registered in the scene
		/// @detail Using a std::string here is less efficient than an enum but more flexible. If
		/// performance needs to be improved, this is a candidate.
		/// @return name of representing action
		auto name() const -> const std::string&;

		/// @brief Categorizes the action by type (usually start or stop)
		/// @return type of the action
		auto type() const -> ActionType;

	  private:
		std::string name_;
		ActionType	type_;
		MousePos	mouse_;
	};
}  // namespace Engine::Systems