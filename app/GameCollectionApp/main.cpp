#include <CLI/App.hpp>		  // NOLINT
#include <CLI/Config.hpp>	  // NOLINT
#include <CLI/Formatter.hpp>  // NOLINT
#include <CodeHelpers/Profiler.hpp>
#include <GameEngine/GameEngine.hpp>
#include <tomlplusplus/toml.hpp>

using namespace std::string_literals;

auto main(int argc, char** argv) -> int
{
	using namespace CodeHelper;

	// The following Initializer for the Game Engine will be modified by the CL parameters
	Engine::Initializer ini;


	//!---------------------------------------------------------------------------------------------
	//! Parse command line arguments
	//!---------------------------------------------------------------------------------------------

	// see https://cliutils.github.io/CLI11/book/
	CLI::App app{"Game collection"};

	auto gameSelect{""s};
	auto inputFile{"../../data/config.toml"s};

	app.add_option("-g,--game", ini.initial_scene, "Which game to load");
	app.add_option("-f,--file", inputFile, "Path to configuration file to load");

	[[maybe_unused]] const auto* list = app.add_flag("-l,--list", "List all available games");
	[[maybe_unused]] const auto* quit = app.add_flag("-q,--quit", "Immediately exit program");

	CLI11_PARSE(app, argc, argv)


	// Read values from the command line or the config.toml file, with the importance being
	// command line > config.timl > default values
	try
	{
		toml::table config = toml::parse_file(inputFile);

		// for each entry in the config.toml file, set the correct value
		if(!ini.initial_scene.has_value())
		{
			ini.initial_scene = config["game"].value<std::string>();
		}
	}
	catch(const std::exception& e)
	{
		std::cerr << e.what() << std::endl;
		return 1;
	}


	// --------------------------------------------------------------------------------------------
	// Start the application
	// --------------------------------------------------------------------------------------------
	if(!quit->as<bool>())
	{
		try
		{
			Engine::GameEngine engine{ini, std::filesystem::path{inputFile}};
			engine.run();
		}
		catch(const std::exception& e)
		{
			std::cerr << e.what() << std::endl;
		}
	}
	else
	{
		std::cout << "Quitting application" << std::endl;
	}
	return 0;
}