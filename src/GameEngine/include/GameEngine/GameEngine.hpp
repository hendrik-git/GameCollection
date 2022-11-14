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
	void quit();
	void change_scene(std::string name, ScenePtr scene, bool end_curr_scene = false);

	auto window() -> sf::RenderWindow&;
	auto assets() -> Assets&;
	auto scenes() -> SceneMap&;
	auto is_running() -> bool;


  protected:
	void init(const fs::path config);
	void update();
	void user_input();
	auto current_scene() -> ScenePtr;


	Assets			 assets_;
	SceneMap		 scenes_;
	sf::RenderWindow window_;
	size_t			 sim_speed_ = 1;
	std::string		 current_scene_;
	bool			 running_ = true;
};