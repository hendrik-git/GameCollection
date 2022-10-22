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
				running_ = false;
			}

			if(event.type == sf::Event::KeyPressed)
			{
				switch(event.key.code)
				{
					case sf::Keyboard::W:
						player_->input->up = true;
						break;
					case sf::Keyboard::A:
						player_->input->left = true;
						break;
					case sf::Keyboard::S:
						player_->input->down = true;
						break;
					case sf::Keyboard::D:
						player_->input->right = true;
						break;
				}
			}

			if(event.type == sf::Event::KeyReleased)
			{
				switch(event.key.code)
				{
					case sf::Keyboard::W:
						player_->input->up = false;
						break;
					case sf::Keyboard::A:
						player_->input->left = false;
						break;
					case sf::Keyboard::S:
						player_->input->down = false;
						break;
					case sf::Keyboard::D:
						player_->input->right = false;
						break;
				}
			}
		}
	}


	void movement()
	{
		// handle player movement
		if(player_->input->up)
		{
			player_->transform->pos.y -= 5.F;
		}
		if(player_->input->down)
		{
			player_->transform->pos.y += 5.F;
		}
		if(player_->input->right)
		{
			player_->transform->pos.x += 5.F;
		}
		if(player_->input->left)
		{
			player_->transform->pos.x -= 5.F;
		}

		for(auto& entity : manager_.get_entities())
		{
			if(auto transf = entity->transform; transf)
			{
				[[maybe_unused]] auto& x   = transf->pos.x;
				[[maybe_unused]] auto& dx  = transf->vel.x;
				[[maybe_unused]] auto& ddx = transf->acc.x;
				[[maybe_unused]] auto& y   = transf->pos.y;
				[[maybe_unused]] auto& dy  = transf->vel.y;
				[[maybe_unused]] auto& ddy = transf->acc.y;

				x += dx;
				y += dy;

				x = std::clamp(x, 0.F, static_cast<float>(window_.getSize().x));
				y = std::clamp(y, 0.F, static_cast<float>(window_.getSize().y));

				transf->angle += 2.F;
			}
		}
	}
	
	void collision() {}
	
	void render()
	{
		window_.clear();

		for(auto entity : manager_.get_entities())
		{
			if(auto shape = entity->shape; shape)
			{
				// assume every shape has also a transform component
				assert(entity->transform && "Missing transform component");

				auto& circle = shape->circle;
				circle.setPosition({entity->transform->pos.x, entity->transform->pos.y});
				circle.setRotation(sf::degrees(entity->transform->angle));

				window_.draw(shape->circle);
			}
		}

		window_.display();
	}

	void spawn_player()
	{
		player_ = manager_.add_entity("player");
		player_->transform =
			std::make_shared<Transform>(Vec2{400.F, 400.F}, Vec2{0.F, 0.F}, Vec2{0.F, 0.F}, 0.F);

		player_->shape = std::make_shared<Shape>(32.F, 8, sf::Color::Black, sf::Color::Red, 4.F);

		player_->input = std::make_shared<Input>();
	}

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