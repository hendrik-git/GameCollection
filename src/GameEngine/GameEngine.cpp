#include "GameEngine/SceneAsteroids.hpp"
#include <GameEngine/GameEngine.hpp>
#include <fmt/core.h>


#pragma region public functions

GameEngine::GameEngine(const fs::path config)
{
	// Load necessary data
	assets_.add_font("Gidole", "../../data/fonts/Gidole.ttf");
	assets_.add_texture("Background", "../../data/Asteroids/Background.png");

	init(config);

	change_scene("Asteroids", std::make_shared<SceneAsteroids>(this));
}

void GameEngine::run()
{
	while(running_)
	{
		user_input();
		scenes_[current_scene_]->update();
	}
}

void GameEngine::quit()
{
	running_ = false;
}

void GameEngine::change_scene(std::string name, ScenePtr scene, bool end_curr_scene)
{
	assert(true);

	if(end_curr_scene)
	{
		/// @todo end current scene here
	}

	scenes_[name]  = scene;
	current_scene_ = name;
}

auto GameEngine::window() -> sf::RenderWindow&
{
	return window_;
}

auto GameEngine::assets() -> Assets&
{
	return assets_;
}

auto GameEngine::is_running() -> bool
{
	return running_;
}
#pragma endregion

#pragma region private functions
void GameEngine::init(const fs::path config)
{
	/// @todo read the config file

	// set up window default parameters
	window_.create(sf::VideoMode({1200, 800}), "GameCollection");
	window_.setFramerateLimit(60);
}

void GameEngine::update() {}

void GameEngine::user_input()
{
	sf::Event event;
	while(window_.pollEvent(event))
	{
		if(event.type == sf::Event::Closed)
		{
			window_.close();
			running_ = false;
		}

		if(event.type == sf::Event::KeyPressed || event.type == sf::Event::KeyReleased)
		{
			auto& actions = current_scene()->get_ActionMap();
			if(actions.find(event.key.code) == actions.end())
			{
				continue;
			}
			const auto type = std::string{(event.type == sf::Event::KeyPressed) ? "Start" : "End"};
			current_scene()->do_action(Action{actions.at(event.key.code), type});
		}

		/// @todo Mouse input is currently not supported yet
		// player_->mouse->lmb = sf::Mouse::isButtonPressed(sf::Mouse::Left);
		// player_->mouse->rmb = sf::Mouse::isButtonPressed(sf::Mouse::Right);
		// player_->mouse->x	= static_cast<float>(sf::Mouse::getPosition(window_).x);
		// player_->mouse->y	= static_cast<float>(sf::Mouse::getPosition(window_).y);
	}
}

auto GameEngine::current_scene() -> ScenePtr
{
	assert(true);
	return scenes_[current_scene_];
}
#pragma endregion