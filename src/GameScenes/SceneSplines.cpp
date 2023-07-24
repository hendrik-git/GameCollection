#include <CodeHelpers/Overload.hpp>
#include <GameEngine/GameEngine.hpp>
#include <GameEngine/Utility.hpp>
#include <GameScenes/SceneSplines.hpp>
#include <cmath>
#include <fmt/core.h>
#include <random>

namespace Engine::Scene
{
	void SceneSplines::init()
	{
		{
			using enum sf::Keyboard::Key;
			register_action(Up, "Up");
			register_action(Left, "Left");
			register_action(Down, "Down");
			register_action(Right, "Right");
			register_action(P, "Pause");
			register_action(Tab, "SwitchPoint");
			register_action(Escape, "Quit");
			register_action(Enter, "Reset");
		}

		auto& texture = game_->assets().get_texture("Background");
		texture.setRepeated(true);
		background_.setPosition({0, 0});
		background_.setTexture(texture);
		background_.setTextureRect(sf::IntRect{
			{0, 0}, {static_cast<int>(world_size_.x), static_cast<int>(world_size_.y)}});

		// spline_.points = {{100, 41}, {200, 41}, {300, 41}, {400, 41}};
		spline_.points = {{100, 501},
						  {200, 241},
						  {400, 141},
						  {600, 341},
						  {800, 141},
						  {900, 141},
						  {1000, 341},
						  {1000, 701}};
	}

	void SceneSplines::update()
	{
		if(paused_)
		{
			return;
		}

		spline_.points.at(selected_point_).y -= static_cast<float>(movement_.up) * 10.f;
		spline_.points.at(selected_point_).y += static_cast<float>(movement_.down) * 10.f;
		spline_.points.at(selected_point_).x -= static_cast<float>(movement_.left) * 10.f;
		spline_.points.at(selected_point_).x += static_cast<float>(movement_.right) * 10.f;
	}

	void SceneSplines::render()
	{
		auto& window = game_->window();

		// from now on, fill the blank canvas
		window.clear();

		// next draw the background (bottom layer)
		window.draw(background_);

		// draw the spline lines
		for(auto t = 0.f; t < static_cast<float>(spline_.points.size() - 0); t += 0.005f)
		{
			// when normal  index -3
			// when looping index -0
			auto pos   = spline_.get_spline_point(t, true);
			auto point = sf::CircleShape(1.f);
			point.setPosition({pos.x, pos.y});
			point.setOrigin({1.f, 1.f});
			window.draw(point);
		}

		// draw all spline points
		for(auto i = 0; i < spline_.points.size(); ++i)
		{
			const auto x_pos = spline_.points.at(i).x;
			const auto y_pos = spline_.points.at(i).y;
			auto	   point = sf::CircleShape(4.f);
			point.setPosition({x_pos, y_pos});
			point.setOrigin({4.f, 4.f});
			window.draw(point);
		}


		// draw the selected spline point again in another color
		{
			const auto x_pos = spline_.points.at(selected_point_).x;
			const auto y_pos = spline_.points.at(selected_point_).y;
			auto	   point = sf::CircleShape(4.f);
			point.setFillColor(sf::Color(0, 255, 0));
			point.setPosition({x_pos, y_pos});
			point.setOrigin({4.f, 4.f});
			window.draw(point);
		}

		// Draw agent to demonstrate gradient
		{
			auto pos	= spline_.get_spline_point(position_, true);
			auto grad	= spline_.get_spline_gradient(position_, true);
			// auto length = std::sqrtf((grad.x * grad.x) + (grad.y + grad.y));
			// auto grad_n = Vec2{grad.x / length, grad.y / length};

			const auto& texture = game_->assets().get_texture("PlayerShip");
			auto		ship	= sf::Sprite(texture);
			ship.setPosition({pos.x, pos.y});
			ship.setOrigin({(float)texture.getSize().x / 2.f, (float)texture.getSize().y / 2.f});

			auto degrees = 180.f / 3.14f * std::atan2(grad.y, grad.x) + 90.f;
			ship.setRotation(sf::degrees(degrees));
			window.draw(ship);

			position_ += 0.01f;
			position_ = std::fmod(position_, spline_.points.size());
		}

		// finally display all rendered content
		window.display();
	}

	void SceneSplines::do_action(const Engine::Systems::Action& action)
	{
		using namespace Engine::Components;
		try
		{
#define MAP_STRING_TO(string, code) \
	if(action.name() == string)     \
	{                               \
		code                        \
	}

			if(action.type() == Engine::Systems::ActionType::Start)
			{
				MAP_STRING_TO("Quit", game_->change_scene("MainMenu"); return;)
				MAP_STRING_TO("Up", movement_.up = true;)
				MAP_STRING_TO("Down", movement_.down = true;)
				MAP_STRING_TO("Left", movement_.left = true;)
				MAP_STRING_TO("Right", movement_.right = true;)
				MAP_STRING_TO("Pause", paused_ = !paused_;)
				MAP_STRING_TO("SwitchPoint",
							  selected_point_ = ++selected_point_ % (spline_.points.size());)
			}
			else if(action.type() == Engine::Systems::ActionType::End)
			{
				MAP_STRING_TO("Up", movement_.up = false;)
				MAP_STRING_TO("Down", movement_.down = false;)
				MAP_STRING_TO("Left", movement_.left = false;)
				MAP_STRING_TO("Right", movement_.right = false;)
			}
#undef MAP_STRING_TO
		}
		catch(const std::exception& e)
		{
			std::cerr << fmt::format("Exception caught {} in {}\n", e.what(), __FILE__);
		}
	}

}  // namespace Engine::Scene