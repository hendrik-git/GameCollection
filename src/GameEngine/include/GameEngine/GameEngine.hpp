/// @file GameEngine

#pragma once
#include "Asset.hpp"
#include "EntityManager.hpp"
#include "Scene.hpp"
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <filesystem>
#include <iostream>

namespace fs   = std::filesystem;
using ScenePtr = std::shared_ptr<Scene>;
using SceneMap = std::map<std::string, ScenePtr>;

class GameEngine
{
  public:
	GameEngine(const fs::path config);

	void run();

	void quit()
	{
		running_ = false;
	};

	void change_scene(std::string name, ScenePtr scene, bool end_curr_scene = false)
	{
		assert(true);

		if(end_curr_scene)
		{
			// end current scene here
		}

		scenes_[name]  = scene;
		current_scene_ = name;
	}

	auto window() -> sf::RenderWindow&
	{
		return window_;
	};
	auto assets() -> Assets&
	{
		return assets_;
	};
	auto is_running() -> bool
	{
		return running_;
	};


  protected:
	void init(const fs::path config);
	void update();
	void user_input();

	auto current_scene() -> ScenePtr
	{
		assert(true);
		return scenes_[current_scene_];
	};

	Assets			 assets_;
	SceneMap		 scenes_;
	sf::RenderWindow window_;
	size_t			 sim_speed_ = 1;
	std::string		 current_scene_;
	bool			 running_ = true;
};