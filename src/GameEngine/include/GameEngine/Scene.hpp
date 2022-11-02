#pragma once
#include "Action.hpp"
#include "EntityManager.hpp"
#include <map>


using ActionMap = std::map<int, std::string>;
using PlayerPtr = std::shared_ptr<Entity>;

class GameEngine;  // forward declare GameEngine class for pointer

class Scene
{
  public:
	Scene() = default;
	Scene(GameEngine* engine);

	virtual void update()						 = 0;
	virtual void render()						 = 0;
	virtual void do_action(const Action& action) = 0;

	// void simulate();
	void register_action(int key, std::string name);

	// size_t	   width();
	// size_t	   height();

	bool	   has_ended();
	size_t	   current_frame();
	ActionMap& get_ActionMap();

	// void	   draw_line(Vec2 p1, Vec2 p2);

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