#include "CLI/App.hpp"
#include "CLI/Config.hpp"
#include "CLI/Formatter.hpp"
#include <GameEngine/GameEngine.hpp>
#include <SFML/Graphics.hpp>

using namespace std::string_literals;

int main(int argc, char** argv)
{
	//!---------------------------------------------------------------------------------------------
	//! Parse command line arguments
	//!---------------------------------------------------------------------------------------------
	// see https://cliutils.github.io/CLI11/book/
	CLI::App app{"Game collection"};

	auto inputFile{"default"s};

	app.add_option("-f,--file", inputFile, "File to load");

	[[maybe_unused]] auto list = app.add_flag("-l,--list", "List all available games");
	[[maybe_unused]] auto quit = app.add_flag("-q,--quit", "Immediately exit program");

	CLI11_PARSE(app, argc, argv);

	// --------------------------------------------------------------------------------------------
	// Start the application
	// --------------------------------------------------------------------------------------------
	if(!quit->as<bool>())
	{
		std::cout << "Test" << std::endl;
		GameEngine engine{std::filesystem::path{}};
		engine.run();
	}
	else
	{
		std::cout << "Quitting application" << std::endl;
	}
	return 0;
}