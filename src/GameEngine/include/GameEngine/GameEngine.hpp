/// @file GameEngine.hpp
/// @brief ...
/// @details ...
/// @see ...
/// @date 2022.11.16
/// @author Hendrik Poettker

#pragma once
#include "Asset.hpp"
#include "EntityManager.hpp"
#include <CodeHelpers/Profiler.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <filesystem>
#include <iostream>


namespace Engine
{
	namespace Scene
	{
		class BaseScene;  // forward declaration
	}

	namespace fs   = std::filesystem;
	using ScenePtr = std::shared_ptr<Engine::Scene::BaseScene>;
	using SceneMap = std::map<std::string, ScenePtr, std::less<>>;


	/// @brief Collection of all initialization parameters for the engine
	struct Initializer
	{
		std::optional<std::string> initial_scene;  //!< used to skip the menu and go to a game
	};


	/// @brief Contains all games and their necessary functionality
	/// @details The game engine contains a collection of scenes (i.e. games). The logic for each
	/// scene is based on the Entity-Component-System design pattern. A scene contains a number of
	/// entities, whose properties are defined by their components. The logic is then implemented
	/// via systems. Finally the scene is rendered to the screen via the SFML functionality.
	class GameEngine
	{
	  public:
		/// @brief Constructor for the game engine, begins loading all common assets and
		/// configurations
		/// @param ini set up the game engine
		/// @param config path to the configuration file
		GameEngine(const Initializer& ini, const fs::path& config);

		/// @brief Starts the game loop and blocks this thread until the game engine closes
		void run();

		/// @brief Closes the game
		void quit();

		/// @brief Changes the scene
		/// @param name of the scene to load
		/// @param end_curr_scene end the previous scene (i.e. reset all entities)
		void change_scene(const std::string& name, bool end_curr_scene = false);

		/// @brief Retuns a reference to the window
		auto window() -> sf::RenderWindow&;

		/// @brief Returns a reference to the manager for loaded assets
		auto assets() -> Asset::Assets&;

		/// @brief Returns a map of the available scenes/games
		auto scenes() -> SceneMap&;

		/// @brief Returns the status of the game engine
		auto is_running() const -> bool;

	  protected:
		void init(const fs::path& config);
		void load_assets();
		void user_input();
		auto current_scene() -> ScenePtr;

	  private:
		Asset::Assets	  assets_;			   //!< holds all assets that are used in the scenes
		sf::RenderWindow  window_;			   //!< the window to render to
		SceneMap		  scenes_;			   //!< all implemented games
		std::string		  current_scene_;	   //!< the currently active scene by name
		size_t			  sim_speed_  = 1;	   //!< how fast the game logic updates (todo)
		std::atomic<bool> running_	  = true;  //!< state of the game engine
		long long		  frame_time_ = 10 * 1'000;	 //!< in microseconds

		CodeHelper::tracing_ptr trace;	//!< holds the perfetto trace

		// frame time of 10 ms equals a refresh rate of 100 Hz
	};

}  // namespace Engine