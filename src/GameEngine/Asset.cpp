#include "GameEngine/Asset.hpp"
#include <cassert>
#include <iostream>

namespace fs = std::filesystem;

void Assets::add_font(std::string name, fs::path path)
{
	assert(!fonts_.contains(name) && "Font already exists");

	if(!fonts_[name].loadFromFile(path))
	{
		throw std::exception(std::format("Failed to load font {}", name).c_str());
	}
}

void Assets::add_texture(std::string name, fs::path path)
{
	assert(!textures_.contains(name) && "Texture already exists");
	// supported image formats are bmp, png, tga, jpg, gif, psd, hdr and pic
	assert(path.extension() == ".png" || path.extension() == ".gif" ||
		   path.extension() == ".jpg" && "Unsupported texture format");

	if(!textures_[name].loadFromFile(path))
	{
		throw std::exception(std::format("Failed to load texture {}", name).c_str());
	}
}

void Assets::add_shader(std::string name, std::filesystem::path path, sf::Shader::Type type)
{
	assert(!shaders_.contains(name) && "Shader already exists");

	if(!shaders_[name].loadFromFile(path, type))
	{
		throw std::exception(std::format("Failed to load shader {}", name).c_str());
	}
}

void Assets::add_sound(const std::string& name, const fs::path& path)
{
	assert(!sounds_.contains(name) && "Sound already exists");
	assert(path.extension() == ".wav" || path.extension() == ".ogg" && "Unsupported sound format");

	if(!sounds_[name].loadFromFile(path))
	{
		throw std::exception(std::format("Failed to load sound {}", name).c_str());
	}
}

auto Assets::get_font(std::string name) -> sf::Font&
{
	assert(fonts_.contains(name) && "Requested font not available");
	return fonts_[name];
}

auto Assets::get_texture(std::string name) -> sf::Texture&
{
	assert(textures_.contains(name) && "Requested texture not available");
	return textures_[name];
}
auto Assets::get_shader(std::string name) -> sf::Shader&
{
	assert(shaders_.contains(name) && "Requested shader not available");
	return shaders_[name];
}

auto Assets::get_sound(std::string name) -> sf::SoundBuffer&
{
	assert(sounds_.contains(name) && "Requested sound not available");
	return sounds_[name];
}


namespace
{
	template<typename T>
	auto get_names_from(const std::map<std::string, T>& map) -> const std::vector<std::string>
	{
		std::vector<std::string> result;
		result.reserve(map.size());

		for(auto& [key, val] : map)
		{
			result.push_back(key);
		}
		return result;
	}
}  // namespace


auto Assets::get_shader_names() const -> const std::vector<std::string>
{
	return get_names_from(shaders_);
}

auto Assets::get_font_names() const -> const std::vector<std::string>
{
	return get_names_from(fonts_);
}

auto Assets::get_texture_names() const -> const std::vector<std::string>
{
	return get_names_from(textures_);
}

auto Assets::get_sound_names() const -> const std::vector<std::string>
{
	return get_names_from(sounds_);
}