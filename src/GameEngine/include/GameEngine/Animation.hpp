/// @file Animation.hpp
/// @brief Defines the Animation class, which is part of the Drawable component
/// @date 2022.11.16
/// @author Hendrik Poettker

#pragma once
#include <GameMath/Vec2.hpp>
#include <SFML/Graphics.hpp>
#include <string>

/// @brief Used as part of the Drawable component, holding the sprite and animation logic
/// @details The components should ideally be free from logic and contain pure data. However this
/// implementation is very narrowly focused and convenient to use.
class Animation
{
  public:
	/// @brief Updates the animation by selecting the next texture rect from the sprite
	void update();

	/// @brief Returns true when the animation has ended
	auto has_ended() const -> bool;

	/// @brief Returns the name of the animation
	auto get_name() const -> const std::string&;

	/// @brief Returns the size of the animation represented by a rectangle
	auto get_size() const -> Vec2;

	/// @brief Returns a reference to the current sprite used for drawing
	auto get_sprite() -> sf::Sprite&;

	/// @brief Returns the current rotation of the sprite
	auto get_rotation() const -> float;

	/// @brief Rotates the animation around its center
	void set_rotation(float degree);

  protected:
	/// @brief Default empty constructor, needed for std::tuple
	Animation() = default;

	/// @brief Constructor for a still image (no animation)
	/// @param name of the animation
	/// @param texture reference to the image in the Asset class
	Animation(std::string name, sf::Texture& texture);

	/// @brief Constructor for an animated image
	/// @param name of the animation
	/// @param texture reference to the image in the Asset class
	/// @param frame_count how many frames this animation consists of
	/// @param speed how fast the animation plays
	Animation(std::string name, sf::Texture& texture, size_t frame_count, size_t speed);

  private:
	sf::Sprite	sprite_;				  //!< holds the current image to be rendered
	size_t		frame_count_   = 1;		  //!< total amount of frames for animation
	size_t		current_frame_ = 0;		  //!< current frame being displayed
	size_t		speed_		   = 0;		  //!< speed to play this animation
	Vec2		size_		   = {1, 1};  //!< how much the sprite is scaled
	float		rotation_	   = 0.F;	  //!< how much the texture is rotated [degree]
	std::string name_;					  //!< name of the animation
};