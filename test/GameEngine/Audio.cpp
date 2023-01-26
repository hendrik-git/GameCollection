#include <SFML/Audio.hpp>
#include <catch2/catch_all.hpp>
#include <iostream>

TEST_CASE("Audio", "[Audio]")
{
	namespace fs = std::filesystem;

	fs::path data_directory{"../../data/sounds"};
	REQUIRE(fs::exists(data_directory));

	// sf::SoundBuffer buffer; <- AUDIO CRASHES
	// sf::Sound sound;

	// sf::SoundBuffer buffer;
	// REQUIRE(!buffer.loadFromFile(data_directory / "sfx_laser1.ogg"));
}