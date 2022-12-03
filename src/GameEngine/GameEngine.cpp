#include "GameEngine/SceneAsteroids.hpp"
#include "GameEngine/SceneMainMenu.hpp"
#include "GameEngine/SceneShaderGallery.hpp"
#include <GameEngine/GameEngine.hpp>
#include <chrono>
#include <filesystem>
#include <fmt/core.h>
#include <functional>

namespace
{
	struct FileData
	{
		std::string filepath;
		std::string filename;
	};

	/// @todo Automate reading all assets in a given directory
	auto find_in_directory(std::filesystem::path path) -> std::vector<FileData>
	{
		std::vector<FileData> results;
		for(const auto& entry : std::filesystem::directory_iterator(path))
		{
			if(entry.is_regular_file())
			{
				results.push_back(FileData{entry.path().string(), entry.path().stem().string()});
			}
		}
		return results;
	}
}  // namespace


#pragma region public functions

GameEngine::GameEngine(const EngineInitializer& ini, const fs::path& config)
{
	// Load necessary data
	assets_.add_font("Gidole", "../../data/fonts/Gidole.ttf");

	std::cout << "Loading textures\n";
	for(auto& texture : find_in_directory("../../data/Asteroids"))
	{
		std::cout << "-- " << texture.filename << std::endl;
		assets_.add_texture(texture.filename, texture.filepath);
	}

	std::cout << "Loading shaders\n";
	for(auto& shader : find_in_directory("../../data/shader"))
	{
		std::cout << "-- " << shader.filename << std::endl;
		assets_.add_shader(shader.filename, shader.filepath);
	}

	init(config);

	scenes_["MainMenu"]		 = std::make_shared<SceneMainMenu>(this);
	scenes_["Asteroids"]	 = std::make_shared<SceneAsteroids>(this);
	scenes_["ShaderGallery"] = std::make_shared<SceneShaderGallery>(this);

	if(const auto& scene = ini.initial_scene.value_or("MainMenu"); !scenes_.contains(scene))
	{
		std::string error_msg{"Can not load specified scene: " + scene};
		throw(std::exception{error_msg.c_str()});
	}

	change_scene(ini.initial_scene.value_or("MainMenu"),
				 scenes_[ini.initial_scene.value_or("MainMenu")]);
}

void GameEngine::run()
{
	using namespace std::chrono;
	while(running_)
	{
		// calculate how long the last frame took
		static auto lag{0.F};
		static auto current{steady_clock::now()};
		static auto previous{current};

		current = steady_clock::now();
		lag += duration_cast<microseconds>(current - previous).count();
		previous = current;


		user_input();

		while(lag >= frame_time_)
		{
			scenes_[current_scene_]->update();
			lag -= frame_time_;
		}
		scenes_[current_scene_]->render();
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

auto GameEngine::scenes() -> SceneMap&
{
	return scenes_;
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
	// window_.setFramerateLimit(60); // replaced by own implementation
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