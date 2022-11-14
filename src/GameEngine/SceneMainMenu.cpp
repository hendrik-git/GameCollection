#include "GameEngine/SceneMainMenu.hpp"
#include "GameEngine/GameEngine.hpp"
#include "GameEngine/Utility.hpp"
#include <fmt/core.h>
#include <random>


void SceneMainMenu::init()
{
	register_action(sf::Keyboard::Up, "Prev");
	register_action(sf::Keyboard::Left, "Prev");
	register_action(sf::Keyboard::Down, "Next");
	register_action(sf::Keyboard::Right, "Next");
	register_action(sf::Keyboard::Space, "Select");
	register_action(sf::Keyboard::Enter, "Select");
	register_action(sf::Keyboard::Escape, "Quit");
}

void SceneMainMenu::update()
{
	entities_.update();
	render();
	current_frame_++;
}

void SceneMainMenu::render()
{
	auto& window	= game_->window();
	auto  view		= window.getDefaultView();
	auto  view_size = view.getSize();
	window.clear();

	// draw background
	window.draw(background_);

	// draw HUD elements on top
	sf::Text text;
	text.setFont(game_->assets().get_font("Gidole"));
	text.setString("Asteroids");
	text.setCharacterSize(48);	// in pixels
	text.setFillColor(sf::Color::White);
	text.setStyle(sf::Text::Bold);
	text.setPosition(window.mapPixelToCoords(
		{static_cast<int>(view_size.x) / 2, static_cast<int>(view_size.y) / 2}, view));
	Utility::center_text(text);
	window.draw(text);

	window.setView(view);
	window.display();
}

void SceneMainMenu::do_action(const Action& action)
{
	if(action.type() == "Start")
	{
		if(action.name() == "Prev")
		{
			player_->get_component<Input>().up = true;
		}
		if(action.name() == "Next")
		{
			player_->get_component<Input>().up = true;
		}
		if(action.name() == "Select")
		{
			player_->get_component<Input>().up = true;
		}
		if(action.name() == "Quit")
		{
			game_->quit();
		}
	}
	else if(action.type() == "End")
	{
		// if(action.name() == "Up")
		//{
		//	player_->get_component<Input>().up = false;
		//}
	}
}

void SceneMainMenu::on_end() {}
