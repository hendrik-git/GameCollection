#include "GameEngine/Asset.hpp"
#include <cassert>

namespace fs = std::filesystem;

void Assets::add_font(std::string name, fs::path path)
{
	assert(fonts_.find(name) == fonts_.end() && "Font already exists");

	sf::Font font;
	if(!font.loadFromFile(path))
	{
		throw std::exception("Failed to load font");
	}

	fonts_[name] = font;
}

void Assets::add_texture(std::string name, fs::path path)
{
	assert(textures_.find(name) == textures_.end() && "Texture already exists");
	// supported image formats are bmp, png, tga, jpg, gif, psd, hdr and pic
	assert(path.extension() == ".png" || path.extension() == ".gif" ||
		   path.extension() == ".jpg" && "Unsupported texture format");

	sf::Texture texture;
	if(!texture.loadFromFile(path))
	{
		throw std::exception("Failed to load font");
	}

	textures_[name] = texture;
}

// void Assets::add_sound(std::string name, fs::path path)
//{
//	assert(sounds_.find(name) == sounds_.end() && "Sound already exists");
//	assert(path.extension() == ".wav" || path.extension() == ".wav" && "Unsupported sound format");
//
//	sf::SoundBuffer sound;
//	if(!sound.loadFromFile(path))
//	{
//		throw std::exception("Failed to load font");
//	}
//
//	sounds_[name] = sound;
//}

auto Assets::get_font(std::string name) -> sf::Font&
{
	assert(fonts_.find(name) != fonts_.end() && "Requested font not available");
	return fonts_[name];
}

auto Assets::get_texture(std::string name) -> sf::Texture&
{
	assert(textures_.find(name) != textures_.end() && "Requested texture not available");
	return textures_[name];
}

// auto Assets::get_sound(std::string name) -> sf::SoundBuffer&
//{
//	assert(sounds_.find(name) != sounds_.end() && "Requested sound not available");
//	return sounds_[name];
//}
