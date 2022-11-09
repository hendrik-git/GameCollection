#include "GameEngine/Animation.hpp"

Animation::Animation() {}

Animation::Animation(std::string name, sf::Texture texture) {}

Animation::Animation(std::string name, sf::Texture texture, size_t frame_count, size_t speed)
	: name_{name}, frame_count_{frame_count}, speed_{speed}
{
	sprite_.setTexture(texture);
}

void Animation::update()
{
	current_frame_++;
	auto frame_number = (speed_ == 0) ? 0 : current_frame_ / speed_ % frame_count_;
	auto frame_rect	  = sf::IntRect{{static_cast<int>(frame_number * size_.x), 0},
									{static_cast<int>(size_.x), static_cast<int>(size_.y)}};
	sprite_.setTextureRect(frame_rect);
}

auto Animation::has_ended() -> bool
{
	return false;
}

auto Animation::get_name() -> std::string
{
	return name_;
}

auto Animation::get_size() -> Vec2
{
	return size_;
}

auto Animation::get_sprite() -> sf::Sprite&
{
	return sprite_;
}
