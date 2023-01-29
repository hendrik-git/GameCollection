#include "GameScenes/SceneParticleGallery.hpp"
#include "GameEngine/GameEngine.hpp"
#include "GameEngine/Utility.hpp"
#include <random>

namespace Engine::Scene
{
	void SceneParticleGallery::init()
	{
		auto& texture = game_->assets().get_texture("Background");
		texture.setRepeated(true);
		background_.setPosition({0, 0});
		background_.setTexture(texture);
		background_.setTextureRect(sf::IntRect{
			{0, 0}, {static_cast<int>(world_size_.x), static_cast<int>(world_size_.y)}});

		register_action(sf::Keyboard::Up, "Prev");
		register_action(sf::Keyboard::Left, "Prev");
		register_action(sf::Keyboard::Down, "Next");
		register_action(sf::Keyboard::Right, "Next");
		register_action(sf::Keyboard::Escape, "Quit");

		for(float x{0}; x < world_size_.x; x += 7.f)
		{
			particles_.create(x, world_size_.y / 2, 0.0, 0.0, 200);
		}
	}

	void SceneParticleGallery::update()
	{
		entities_.update();
		particles_.update();
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

	void SceneParticleGallery::render()
	{
		auto& window	= game_->window();
		auto  view		= window.getDefaultView();
		auto  view_size = view.getSize();
		window.clear();
		window.setView(view);

		// draw background
		window.draw(background_);

		// draw particles
		particles_.draw(window);

		// draw name of the scene at the top
		auto pos_x = static_cast<int>(view_size.x) / 2;		 // horizontal center
		auto pos_y = static_cast<int>(view_size.y) * 1 / 4;	 // top quarter
		auto pos   = window.mapPixelToCoords({pos_x, pos_y}, view);
		draw_text(game_, "Particle Gallery", pos, 48, true);

		// finally display all rendered content
		window.display();
	}

	void SceneParticleGallery::do_action([[maybe_unused]] const Engine::Systems::Action& action)
	{
		auto print_current_shader_name = [&]()
		{
			if(const auto names = game_->assets().get_shader_names(); !names.empty())
			{
				selection_ = selection_ % names.size();
				std::cout << "Selected shader: " << names[selection_] << std::endl;
			}
		};

		if(action.type() == Engine::Systems::ActionType::Start)
		{
			if(action.name() == "Prev")
			{
				selection_--;
			}
			if(action.name() == "Next")
			{
				selection_++;
			}
			if(action.name() == "Quit")
			{
				game_->quit();
			}
		}
	}

	void SceneParticleGallery::on_end() {}

}  // namespace Engine::Scene