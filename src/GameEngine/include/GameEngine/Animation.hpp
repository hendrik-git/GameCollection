#pragma once
#include <GameMath/Vec2.hpp>
#include <SFML/Graphics.hpp>
#include <string>

class Animation
{
  public:
	Animation();
	Animation(std::string name, sf::Texture& texture);
	Animation(std::string name, sf::Texture& texture, size_t frame_count, size_t speed);

	void update();
	auto has_ended() -> bool;
	auto get_name() -> std::string;
	auto get_size() -> Vec2;
	auto get_sprite() -> sf::Sprite&;

  private:
	sf::Sprite	sprite_;
	size_t		frame_count_   = 1;		  ///< total amount of frames for animation
	size_t		current_frame_ = 0;		  ///< current frame being displayed
	size_t		speed_		   = 0;		  ///< speed to play this animation
	Vec2		size_		   = {1, 1};  ///< how much the sprite is scaled
	std::string name_;
};