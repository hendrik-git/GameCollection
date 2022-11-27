#include "GameEngine/SceneShaderGallery.hpp"
#include "GameEngine/GameEngine.hpp"
#include "GameEngine/Utility.hpp"
#include <fmt/core.h>
#include <random>


void SceneShaderGallery::init()
{
	register_action(sf::Keyboard::Up, "Prev");
	register_action(sf::Keyboard::Left, "Prev");
	register_action(sf::Keyboard::Down, "Next");
	register_action(sf::Keyboard::Right, "Next");
	register_action(sf::Keyboard::Space, "Select");
	register_action(sf::Keyboard::Enter, "Select");
	register_action(sf::Keyboard::Escape, "Quit");
}

void SceneShaderGallery::update()
{
	entities_.update();
	current_frame_++;
}

void SceneShaderGallery::render()
{
	auto& window	= game_->window();
	auto  view		= window.getDefaultView();
	auto  view_size = view.getSize();
	window.clear();
	window.setView(view);

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

	// finally display all rendered content
	window.display();
}

void SceneShaderGallery::do_action([[maybe_unused]] const Action& action)
{

}

void SceneShaderGallery::on_end() {}
