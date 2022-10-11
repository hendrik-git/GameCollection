#include "CLI/App.hpp"
#include "CLI/Config.hpp"
#include "CLI/Formatter.hpp"
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

	auto quit = app.add_flag("-q,--quit", "Immediately exit program");

	CLI11_PARSE(app, argc, argv);

	// --------------------------------------------------------------------------------------------
	// Start the application
	// --------------------------------------------------------------------------------------------
	if(!quit->as<bool>())
	{
		std::cout << "Starting application" << std::endl;

		sf::RenderWindow window(sf::VideoMode({200, 200}), "SFML works!");
		sf::CircleShape	 shape(100.f);
		shape.setFillColor(sf::Color::Green);

		while(window.isOpen())
		{
			sf::Event event;
			while(window.pollEvent(event))
			{
				if(event.type == sf::Event::Closed)
					window.close();
			}

			window.clear();
			window.draw(shape);
			window.display();
		}
	}
	else
	{
		std::cout << "Quitting application" << std::endl;
	}

	return 0;
}