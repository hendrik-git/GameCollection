/// @file Asset.hpp
/// @brief Defines the Assets class, that stores fonts, textures and sounds
/// @details ...
/// @see ...
/// @date 2022.11.16
/// @author Hendrik Poettker

#pragma once
#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <filesystem>
#include <map>
#include <string>

/// @brief Class that stores all fonts, textures and sounds.
/// @todo add Animation assets
class Assets
{
  public:
	Assets() = default;

	void add_font(std::string name, std::filesystem::path path);
	void add_texture(std::string name, std::filesystem::path path);
	void add_shader(std::string			  name,
					std::filesystem::path path,
					sf::Shader::Type	  type = sf::Shader::Type::Fragment);
	void add_sound(const std::string& name, const std::filesystem::path& path);

	[[nodiscard]] auto get_font(std::string name) -> sf::Font&;
	[[nodiscard]] auto get_texture(std::string name) -> sf::Texture&;
	[[nodiscard]] auto get_shader(std::string name) -> sf::Shader&;
	[[nodiscard]] auto get_sound(std::string name) -> sf::SoundBuffer&;

	// void add_animation();
	// [[nodiscard]] auto get_animation() -> Animation&;

	/// @brief Returns a vector of names (keys) for the stored shaders
	[[nodiscard]] auto get_shader_names() const -> const std::vector<std::string>;

	/// @brief Returns a vector of names (keys) for the stored fonts
	[[nodiscard]] auto get_font_names() const -> const std::vector<std::string>;

	/// @brief Returns a vector of names (keys) for the stored textures
	[[nodiscard]] auto get_texture_names() const -> const std::vector<std::string>;

	/// @brief Returns a vector of names (keys) for the stored sounds
	[[nodiscard]] auto get_sound_names() const -> const std::vector<std::string>;

  private:
	std::map<std::string, sf::Font>		   fonts_;	   //!< holds all fonts by name
	std::map<std::string, sf::Texture>	   textures_;  //!< holds all textures by name
	std::map<std::string, sf::Shader>	   shaders_;   //!< holds all shaders by name
	std::map<std::string, sf::SoundBuffer> sounds_;	   //!< holds all sounds by name
	// std::map<std::string, Animation> animations_;
};