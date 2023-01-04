/// @file Animation.hpp
/// @brief ...
/// @details ...
/// @see ...
/// @date 2022.11.16
/// @author Hendrik Poettker

#pragma once
#include <GameMath/Vec2.hpp>
#include <SFML/Graphics.hpp>
#include <string>

/// @brief Used as part of the Drawable component, holding the sprite and animation logic
/// @detail The components should ideally be free from logic and contain pure data. However this
/// implementation is very narrowly focused and convenient to use.
class Animation
{
  public:
	void update();
	auto has_ended() -> bool;
	auto get_name() -> std::string;
	auto get_size() -> Vec2;
	auto get_sprite() -> sf::Sprite&;
	auto get_rotation() -> float;
	void set_rotation(float degree);

  protected:
	Animation() = default;
	Animation(std::string name, sf::Texture& texture);
	Animation(std::string name, sf::Texture& texture, size_t frame_count, size_t speed);

  private:
	sf::Sprite	sprite_;
	size_t		frame_count_   = 1;		  ///< total amount of frames for animation
	size_t		current_frame_ = 0;		  ///< current frame being displayed
	size_t		speed_		   = 0;		  ///< speed to play this animation
	Vec2		size_		   = {1, 1};  ///< how much the sprite is scaled
	float		rotation_	   = 0.F;	  ///< how much the texture is rotated [degree]
	std::string name_;
};