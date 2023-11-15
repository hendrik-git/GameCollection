#include <CLI/App.hpp>		  // NOLINT
#include <CLI/Config.hpp>	  // NOLINT
#include <CLI/Formatter.hpp>  // NOLINT
#include <CodeHelpers/Profiler.hpp>
#include <GameEngine/GameEngine.hpp>
#include <tomlplusplus/toml.hpp>
// #include <CodeHelpers/Logger.hpp>
#include <quill/Quill.h>

using namespace std::string_literals;

auto main(int argc, char** argv) -> int
{
	using namespace CodeHelper;

	quill::Config cfg;
	cfg.enable_console_colours = true;
	quill::configure(cfg);
	quill::start();
	// Default root logger to stdout
	quill::Logger* logger = quill::get_logger();
	logger->set_log_level(quill::LogLevel::TraceL3);
	// enable a backtrace that will get flushed when we log CRITICAL
	logger->init_backtrace(2u, quill::LogLevel::Critical);
	LOG_BACKTRACE(logger, "Backtrace log {}", 1);
	LOG_BACKTRACE(logger, "Backtrace log {}", 2);
	LOG_INFO(logger, "Welcome to Quill!");
	LOG_ERROR(logger, "An error message. error code {}", 123);
	LOG_WARNING(logger, "A warning message.");
	LOG_CRITICAL(logger, "A critical error.");
	LOG_DEBUG(logger, "Debugging foo {}", 1234);
	LOG_TRACE_L1(logger, "{:>30}", "right aligned");
	LOG_TRACE_L2(logger, "Positional arguments are {1} {0} ", "too", "supported");
	LOG_TRACE_L3(logger, "Support for floats {:03.2f}", 1.23456);

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