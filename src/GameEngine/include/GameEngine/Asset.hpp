
#pragma once
//#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <filesystem>
#include <map>
#include <string>

class Assets
{
  public:
	Assets() = default;

	void add_font(std::string name, std::filesystem::path path);
	void add_texture(std::string name, std::filesystem::path path);
	// void add_sound(std::string name, std::filesystem::path path);

	[[nodiscard]] auto get_font(std::string name) -> sf::Font&;
	[[nodiscard]] auto get_texture(std::string name) -> sf::Texture&;
	//[[nodiscard]] auto get_sound(std::string name) -> sf::SoundBuffer&;

	// void add_animation();
	// [[nodiscard]] auto get_animation() -> Animation&;

  private:
	std::map<std::string, sf::Font>	   fonts_;
	std::map<std::string, sf::Texture> textures_;
	// std::map<std::string, sf::SoundBuffer> sounds_;

	// std::map<std::string, Animation> animations_;
};