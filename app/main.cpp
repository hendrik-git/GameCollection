#include <CLI/App.hpp>
#include <CLI/Config.hpp>
#include <CLI/Formatter.hpp>
#include <GameEngine/GameEngine.hpp>

using namespace std::string_literals;

int main(int argc, char** argv)
{
	//!---------------------------------------------------------------------------------------------
	//! Parse command line arguments
	//!---------------------------------------------------------------------------------------------
	// see https://cliutils.github.io/CLI11/book/
	CLI::App app{"Game collection"};

	auto gameSelect{""s};
	auto inputFile{"default"s};

	app.add_option("-g,--game", gameSelect, "Which game to load");
	app.add_option("-f,--file", inputFile, "Settings file to load");

	[[maybe_unused]] auto list = app.add_flag("-l,--list", "List all available games");
	[[maybe_unused]] auto quit = app.add_flag("-q,--quit", "Immediately exit program");

	CLI11_PARSE(app, argc, argv);


	// --------------------------------------------------------------------------------------------
	// Start the application
	// --------------------------------------------------------------------------------------------
	if(!quit->as<bool>())
	{
		try
		{
			GameEngine engine{std::filesystem::path{inputFile}};
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