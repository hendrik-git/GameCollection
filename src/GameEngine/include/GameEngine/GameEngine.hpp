/// @file GameEngine

#pragma once
#include "EntityManager.hpp"
#include <SFML/Graphics.hpp>

class GameEngine
{
  public:
	void init() {}

	void main_loop()
	{
		current_frame_++;
		manager_.update();
		user_input();
		movement();
		collision();
		render();
	}

	void user_input() {}
	void movement() {}
	void collision() {}
	void render() {}

  private:
	sf::RenderWindow window_;
	EntityManager	 manager_;
	Entity			 player_;
	bool			 paused_;
	bool			 running_;
	size_t			 current_frame_ = 0;
};