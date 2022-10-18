/// @file GameEngine

#pragma once
#include "EntityManager.hpp"
#include <SFML/Graphics.hpp>

class GameEngine
{
  public:
	GameEngine()
	{
		window_.create(sf::VideoMode({400, 400}), "GameCollection");
	}

	void init() {}

	void main_loop()
	{
		while(window_.isOpen())
		{
			current_frame_++;
			manager_.update();
			user_input();
			movement();
			collision();
			render();
		}
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
	bool			 paused_		= false;
	bool			 running_		= false;
	size_t			 current_frame_ = 0;
	std::shared_ptr<Entity> player_; ///< for convenient access to the player entity
};