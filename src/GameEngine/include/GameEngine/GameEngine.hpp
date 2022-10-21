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
	GameEngine(const fs::path config)
	{
		init(config);
	}

	void run()
	{
		/// @todo add a pause system
		// pause system should only pause some systems (movement / input)

		while(running_)
		{
			manager_.update();
			user_input();
			movement();
			collision();
			render();

			current_frame_++;
		}
	}

  private:
	void init(const fs::path config)
	{
		/// @todo read the config file

		// set up window default parameters
		window_.create(sf::VideoMode({1200, 800}), "GameCollection");
		window_.setFramerateLimit(60);

		spawn_player();
	}

	void user_input()
	{
		sf::Event event;
		while(window_.pollEvent(event))
		{
			if(event.type == sf::Event::Closed)
			{
				window_.close();
			}
		}
	}
	void movement() {}
	void collision() {}
	void render()
	{
		sf::CircleShape shape(100.f);
		shape.setFillColor(sf::Color::Green);

		window_.clear();
		window_.draw(shape);
		window_.display();
	}

  private:
	sf::RenderWindow window_;
	EntityManager	 manager_;
	sf::Font		 font_;
	sf::Text		 text_;
	bool			 paused_		= false;
	bool			 running_		= true;
	size_t			 current_frame_ = 0;
	int				 score			= 0;

	// PlayerConfig
	// EnemyConfig
	// BulletConfig

	std::shared_ptr<Entity> player_;  ///< for convenient access to the player entity
};