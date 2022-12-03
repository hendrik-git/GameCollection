#include "GameEngine/SceneShaderGallery.hpp"
#include "GameEngine/GameEngine.hpp"
#include "GameEngine/Utility.hpp"
#include <fmt/core.h>
#include <random>

void SceneShaderGallery::spawn_player()
{
	player_ = entities_.add_entity("player");
	player_->add_component<Transform>(Vec2{world_size_.x / 2, world_size_.y / 2});


	ShapeInit player_shape;
	player_shape.radius	   = 12.F;
	player_shape.points	   = 32;
	player_shape.fill	   = sf::Color::Yellow;
	player_shape.outline   = sf::Color::White;
	player_shape.thickness = 4.F;
	// player_->add_component<Shape>(player_shape);
	// player_->add_component<Input>();
	// player_->add_component<Mouse>();

	player_->add_component<Drawable>("PlayerShip", game_->assets().get_texture("Meteor9"));
}


void SceneShaderGallery::init()
{
	register_action(sf::Keyboard::Up, "Prev");
	register_action(sf::Keyboard::Left, "Prev");
	register_action(sf::Keyboard::Down, "Next");
	register_action(sf::Keyboard::Right, "Next");
	register_action(sf::Keyboard::Space, "Select");
	register_action(sf::Keyboard::Enter, "Select");
	register_action(sf::Keyboard::Escape, "Quit");

	spawn_player();
}

void SceneShaderGallery::update()
{
	entities_.update();
	current_frame_++;
}

namespace
{
	void draw_text(GameEngine*		   game_,
				   const std::string&  text,
				   const sf::Vector2f& pos,
				   const int		   size = 24,
				   bool				   bold = false)
	{
		auto&	 window = game_->window();
		sf::Text result;
		result.setFont(game_->assets().get_font("Gidole"));
		result.setString(text);
		result.setCharacterSize(size);	// in pixels
		result.setFillColor(sf::Color::White);
		if(bold)
		{
			result.setStyle(sf::Text::Bold);
		}
		result.setPosition(pos);
		Utility::center_text(result);
		window.draw(result);
	}
}  // namespace

void SceneShaderGallery::render()
{
	auto& window	= game_->window();
	auto  view		= window.getDefaultView();
	auto  view_size = view.getSize();
	window.clear();
	window.setView(view);

	// draw background
	window.draw(background_);


	// draw name of the scene at the top
	auto pos_x = static_cast<int>(view_size.x) / 2;		 // horizontal center
	auto pos_y = static_cast<int>(view_size.y) * 1 / 4;	 // top quarter
	auto pos   = window.mapPixelToCoords({pos_x, pos_y}, view);
	draw_text(game_, "Shader Gallery", pos, 48, true);

	// finally display all rendered content
	window.display();
}

void SceneShaderGallery::do_action([[maybe_unused]] const Action& action)
{

}

void SceneShaderGallery::on_end() {}
