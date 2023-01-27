#include <CodeHelpers/Profiler.hpp>
#include <GameEngine/GameEngine.hpp>
#include <GameScenes/SceneAsteroids.hpp>
#include <GameScenes/SceneMainMenu.hpp>
#include <GameScenes/SceneParticleGallery.hpp>
#include <GameScenes/SceneShaderGallery.hpp>
#include <chrono>
#include <filesystem>
#include <fmt/core.h>
#include <functional>
#include <future>


namespace
{
	struct FileData
	{
		std::string filepath;
		std::string filename;
	};

	/// @todo Automate reading all assets in a given directory
	auto find_in_directory(fs::path path) -> std::vector<FileData>
	{
		std::vector<FileData> results;
		for(const auto& entry : fs::directory_iterator(path))
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
	// Set up tracing
	CodeHelper::InitializePerfetto();
	trace = CodeHelper::StartTracing();
	TRACE_EVENT("engine", "GameEngine()");

	// Load all assets asynchronously while initializing the engine
	auto asset_fut = std::async(std::launch::async, &GameEngine::load_assets, this);
	
	// Load configuration and create the main window
	init(config);

	asset_fut.wait(); // merge threads again


	using namespace Engine::Scene;
	scenes_["MainMenu"]		   = std::make_shared<SceneMainMenu>(this);
	scenes_["Asteroids"]	   = std::make_shared<SceneAsteroids>(this);
	scenes_["ParticleGallery"] = std::make_shared<SceneParticleGallery>(this);
	scenes_["ShaderGallery"]   = std::make_shared<SceneShaderGallery>(this);

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
	TRACE_EVENT("engine", "run()");
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

		{
			TRACE_EVENT("engine", "user_input()");
			user_input();
		}

		while(lag >= frame_time_)
		{
			TRACE_COUNTER("engine", "Lag", lag);
			TRACE_EVENT("engine", "update()");
			scenes_[current_scene_]->update();
			lag -= frame_time_;
		}

		{
			TRACE_EVENT("rendering", "render()");
			scenes_[current_scene_]->render();
		}
	}
}

void GameEngine::quit()
{
	CodeHelper::StopTracing(std::move(trace));

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
	TRACE_EVENT("engine", "init()");
	/// @todo read the config file

	// set up window default parameters
	// this need to happen in the main thread, or opengl complains
	window_.create(sf::VideoMode({1200, 800}), "GameCollection");
}

void GameEngine::load_assets()
{
	{
		TRACE_EVENT("engine", "Loading fonts");
		assets_.add_font("Gidole", "../../data/fonts/Gidole.ttf");
	}

	{
		TRACE_EVENT("engine", "Loading textures");
		std::cout << "Loading textures\n";
		for(auto& texture : find_in_directory("../../data/Asteroids"))
		{
			std::cout << "-- " << texture.filename << std::endl;
			assets_.add_texture(texture.filename, texture.filepath);
		}
	}

	{
		TRACE_EVENT("engine", "Loading shaders");
		std::cout << "Loading shaders\n";
		for(auto& shader : find_in_directory("../../data/shader"))
		{
			std::cout << "-- " << shader.filename << std::endl;
			assets_.add_shader(shader.filename, shader.filepath);
		}
	}

	TRACE_EVENT("engine", "task finished");
}

void GameEngine::update() {}

void GameEngine::user_input()
{
	sf::Event event;
	auto&	  actions = current_scene()->get_ActionMap();

	while(window_.pollEvent(event))
	{
		using namespace Engine::Systems;

		if(event.type == sf::Event::Closed)
		{
			window_.close();
			running_ = false;
		}

		if(event.type == sf::Event::KeyPressed || event.type == sf::Event::KeyReleased)
		{
			if(actions.find(event.key.code) == actions.end())
			{
				continue;  // ignore events, that the scene does not use
			}

			const auto type =
				event.type == sf::Event::KeyPressed ? ActionType::Start : ActionType::End;
			current_scene()->do_action(Action{actions.at(event.key.code), type});
		}

		if(event.type == sf::Event::MouseButtonPressed ||
		   event.type == sf::Event::MouseButtonReleased)
		{
			if(actions.find(event.key.code) == actions.end())
			{
				continue;  // ignore events, that the scene does not use
			}
			auto pos = MousePos{{static_cast<float>(sf::Mouse::getPosition(window_).x),
								 static_cast<float>(sf::Mouse::getPosition(window_).y)}};

			const auto type =
				event.type == sf::Event::MouseButtonPressed ? ActionType::Start : ActionType::End;

			current_scene()->do_action(Action{actions.at(event.key.code), type, pos});
		}
	}
}

auto GameEngine::current_scene() -> ScenePtr
{
	assert(true);
	return scenes_[current_scene_];
}
#pragma endregion