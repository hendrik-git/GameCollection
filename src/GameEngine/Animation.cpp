#include "GameEngine/Animation.hpp"

namespace Engine::Components
{
	Animation::Animation(std::string name, const sf::Texture& texture) : name_{std::move(name)}
	{
		auto size = static_cast<sf::Vector2f>(texture.getSize());
		sprite_.setOrigin({size.x / 2, size.y / 2});
		sprite_.setTexture(texture);
	}

	Animation::Animation(std::string name, const sf::Texture& texture, size_t frame_count, size_t speed)
		: name_{std::move(name)}, frame_count_{frame_count}, speed_{speed}
	{
		auto size = static_cast<sf::Vector2f>(texture.getSize());
		sprite_.setOrigin({size.x / 2, size.y / 2});
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

	auto Animation::has_ended() const -> bool
	{
		return false;
	}

	auto Animation::get_name() const -> const std::string&
	{
		return name_;
	}

	auto Animation::get_size() const -> Vec2
	{
		return size_;
	}

	auto Animation::get_sprite() -> sf::Sprite&
	{
		return sprite_;
	}

	auto Animation::get_rotation() const -> float
	{
		return rotation_;
	}

	void Animation::set_rotation(float degree)
	{
		rotation_ = degree;
	}

}  // namespace Engine::Components