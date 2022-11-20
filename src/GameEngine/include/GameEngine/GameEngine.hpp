/// @file GameEngine.hpp
/// @brief ...
/// @details ...
/// @see ...
/// @date 2022.11.16
/// @author Hendrik Pöttker

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
	sf::RenderWindow window_;
	SceneMap		 scenes_;
	std::string		 current_scene_;
	size_t			 sim_speed_	 = 1;
	bool			 running_	 = true;
	long long		 frame_time_ = 10 * 1'000;  ///< in microseconds

	// frame time of 10 ms equals a refresh rate of 100 Hz
};