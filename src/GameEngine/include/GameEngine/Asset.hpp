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
	/// @brief Default constructor
	Assets() = default;

	/// @brief Loads a font from a file and adds it to the assets data
	/// @param name how the font will be refered to internally
	/// @param path path to the font file
	void add_font(std::string name, std::filesystem::path path);

	/// @brief Loads a texture from a file and adds it to the assets data
	/// @param name how the texture will be refered to internally
	/// @param path path to the texture file
	void add_texture(std::string name, std::filesystem::path path);

	/// @brief Loads a shader from a file and adds it to the assets data
	/// @param name how the shader will be refered to internally
	/// @param path path to the shader file
	/// @param type either Fragment, Vertex or Geometry shader
	void add_shader(std::string			  name,
					std::filesystem::path path,
					sf::Shader::Type	  type = sf::Shader::Type::Fragment);

	/// @brief Loads a sound from a file and adds it to the assets data
	/// @param name how the sound will be refered to internally
	/// @param path path to the sound file
	void add_sound(const std::string& name, const std::filesystem::path& path);

	/// @brief Returns a reference to a loaded font
	/// @details e.g. for use with sf::Text
	/// @param name of the font to load
	[[nodiscard]] auto get_font(std::string name) -> sf::Font&;

	/// @brief Returns a reference to a loaded texture
	/// @details e.g. for use with sf::Sprite
	/// @param name of the texture to load
	[[nodiscard]] auto get_texture(std::string name) -> sf::Texture&;

	/// @brief Returns a reference to a loaded shader
	/// @details use with GameEngine::Component::Shader
	/// @param name of the font to shader
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