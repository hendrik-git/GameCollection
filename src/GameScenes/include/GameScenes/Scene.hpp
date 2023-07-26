/// @file Scene.hpp
/// @brief ...
/// @details ...
/// @see ...
/// @date 2022.11.16
/// @author Hendrik Poettker

#pragma once
#include "GameEngine/Action.hpp"
#include "GameEngine/EntityManager.hpp"
#include <entt/entt.hpp>
#include <map>

namespace Engine
{
	using ActionMap = std::map<int, std::string>;  //!< for registering all available actions
	using PlayerPtr = std::shared_ptr<Entity>;	   //!< shorthand for getting player entity
	class GameEngine;							   // forward declare GameEngine class for pointer

	/// @brief Holds all implementations for games as scenes
	namespace Scene
	{
		/// @brief Base for all scenes to be derived from
		class BaseScene
		{
		  public:
			/// @brief Constructor with reference to the GameEngine
			/// @param engine pointer to the game engine instance from where to get assets etc.
			explicit BaseScene(GameEngine* engine);

			/// @brief Default constructor (should not be used)
			BaseScene() = default;

			/// @brief Polymorphic base class destructor should be either public virtual
			virtual ~BaseScene() = default;

			/// @brief For implementing and advancing the scene logic
			virtual void update() = 0;

			/// @brief For implementing the drawing of the scene to the window
			virtual void render() = 0;

			/// @brief For implementing the reaction of the scene to input
			/// @param action abstracted input
			virtual void do_action(const Systems::Action& action) = 0;

			/// @brief For registering all inputs the scene can use
			/// @param key what the input is (see SFML KeyCodes)
			/// @param name how the input may be adressed
			void register_action(int key, const std::string& name);

			/// @brief Returns all registered inputs this scene can use
			auto get_ActionMap() -> ActionMap&;

			/// @brief Returns the current state of the scene
			bool has_ended() const noexcept;

			/// @brief Returns the current frame, each update() call increments this value
			size_t current_frame() const noexcept;


		  protected:
			virtual void on_end() = 0;
			void		 set_paused(bool pause);

			Engine::GameEngine* game_ = nullptr;
			PlayerPtr			player_;
			ActionMap			action_map_;
			EntityManager		entities_;
			bool				is_paused_	   = false;
			bool				has_ended_	   = false;
			size_t				current_frame_ = 0;

			entt::registry registry_;
		};
	}  // namespace Scene

}  // namespace Engine