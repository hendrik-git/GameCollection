/// @file GameEngine

#pragma once
#include "EntityManager.hpp"

class GameEngine
{
  public:
	void main_loop()
	{
		manager_.update();
		// user input
		// movement
		// collision
		// render
		current_frame_++;
	}

  private:
	EntityManager manager_;
	size_t		  current_frame_ = 0;
};