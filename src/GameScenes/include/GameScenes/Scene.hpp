/// @file Scene.hpp
/// @brief ...
/// @details ...
/// @see ...
/// @date 2022.11.16
/// @author Hendrik Poettker

#pragma once
#include "GameEngine/Action.hpp"
#include "GameEngine/EntityManager.hpp"
#include <map>


using ActionMap = std::map<int, std::string>;
using PlayerPtr = std::shared_ptr<Entity>;
class GameEngine;  // forward declare GameEngine class for pointer

namespace Engine::Scene
{
	class BaseScene
	{
	  public:
		BaseScene() = default;
		BaseScene(GameEngine* engine);

		virtual void update()										  = 0;
		virtual void render()										  = 0;
		virtual void do_action(const Engine::Systems::Action& action) = 0;
		void		 register_action(int key, std::string name);
		bool		 has_ended();
		size_t		 current_frame();
		ActionMap&	 get_ActionMap();

	  protected:
		virtual void on_end() = 0;
		void		 set_paused(bool pause);

		PlayerPtr	  player_;
		ActionMap	  action_map_;
		EntityManager entities_;
		GameEngine*	  game_			 = nullptr;
		bool		  is_paused_	 = false;
		bool		  has_ended_	 = false;
		size_t		  current_frame_ = 0;
	};
}  // namespace Engine::Scene