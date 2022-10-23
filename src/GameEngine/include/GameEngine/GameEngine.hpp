/// @file GameEngine

#pragma once
#include "EntityManager.hpp"
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <filesystem>
#include <iostream>

namespace fs = std::filesystem;

class GameEngine
{
  public:
	GameEngine(const fs::path config);

	void run();

  private:
	void init(const fs::path config);

	void user_input();

	void spawn_entities();

	void reduce_lifespan();

	void movement();

	void collision() {}

	void render();

	void spawn_player();

	sf::RenderWindow window_;
	EntityManager	 manager_;
	sf::Font		 font_;
	sf::Text		 text_;
	bool			 paused_		= false;
	bool			 running_		= true;
	size_t			 current_frame_ = 0;
	int				 score			= 0;

	// for convenient access to the player entity
	std::shared_ptr<Entity> player_;
};