#include "GameScenes/SceneShaderGallery.hpp"
#include "GameEngine/GameEngine.hpp"
#include "GameEngine/Utility.hpp"
#include <CodeHelpers/Overload.hpp>
#include <random>

namespace Engine::Scene
{
	void SceneShaderGallery::spawn_player()
	{
		using namespace Engine::Components;

		player_ = entities_.add_entity("player");
		player_->add_component<Transform>(Vec2{world_size_.x / 2, world_size_.y / 2});
		player_->add_component<Drawable>("PlayerShip", game_->assets().get_texture("Meteor9"));

		ShaderParamMap parameters;
		parameters["u_time"]  = 10.F;
		parameters["texture"] = nullptr;

		player_->add_component<Shader>(ShaderName{"blur"}, parameters);
	}


	void SceneShaderGallery::init()
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
		// register_action(sf::Keyboard::Space, "Select");
		// register_action(sf::Keyboard::Enter, "Select");
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
		using namespace Engine::Components;

		static auto clock	  = sf::Clock{};
		auto&		window	  = game_->window();
		auto		view	  = window.getDefaultView();
		auto		view_size = view.getSize();
		window.clear();
		window.setView(view);

		// draw background
		window.draw(background_);

		// then fill the vastness of space with entities (space ship, lasers, asteroids..)
		for(auto& entity : entities_.get_entities())
		{
			if(auto& shape = entity->get_component<Drawable>(); shape)
			{
				// assume every shape has also a transform component
				assert(entity->get_component<Transform>() && "Missing transform component");
				auto  sprite = shape.get_sprite();
				auto& transf = entity->get_component<Transform>();

				sprite.setPosition({transf.pos.x, transf.pos.y});
				sprite.setRotation(sf::degrees(transf.angle + shape.get_rotation()));

				// FIRST ITERATION WITH SHADER AS ARRAY
				// if(const auto names = game_->assets().get_shader_names(); !names.empty())
				//{
				//	[[maybe_unused]] auto& pixel_shader =
				//		game_->assets().get_shader(names[selection_]);

				//	pixel_shader.setUniform("texture", sf::Shader::CurrentTexture);
				//	pixel_shader.setUniform("u_time", clock.getElapsedTime().asSeconds());
				//	window.draw(sprite, &pixel_shader);

				//	// draw the name of the shader under the shader image
				//	auto pos_x = static_cast<int>(view_size.x) / 2;		 // horizontal center
				//	auto pos_y = static_cast<int>(view_size.y) * 3 / 4;	 // bottom quarter
				//	auto pos   = window.mapPixelToCoords({pos_x, pos_y}, view);
				//	// draw_text(game_, names[selection_], pos);
				//}
				// END FIRST

				if(auto& shader = entity->get_component<Shader>(); shader)
				{
					[[maybe_unused]] const auto& pixel_shader =
						game_->assets().get_shader(shader.name);

					for(auto& [param_name, param_value] : shader.parameters)
					{
						 auto visitable = CodeHelper::Overload{
							[&](float value)
							{
								game_->assets()
									.get_shader(shader.name)
									.setUniform(param_name, value);
							},
							[&](int value)
							{
								game_->assets()
									.get_shader(shader.name)
									.setUniform(param_name, value);
							},
							[&](const sf::Texture*)
							{
								game_->assets()
									.get_shader(shader.name)
									.setUniform(param_name, sf::Shader::CurrentTexture);
							}};

						std::visit(visitable, param_value);
					}

					window.draw(sprite, &pixel_shader);
				}
				else
				{
					window.draw(sprite);
				}
			}
		}

		// draw name of the scene at the top
		auto pos_x = static_cast<int>(view_size.x) / 2;		 // horizontal center
		auto pos_y = static_cast<int>(view_size.y) * 1 / 4;	 // top quarter
		auto pos   = window.mapPixelToCoords({pos_x, pos_y}, view);
		// draw_text(game_, "Shader Gallery", pos, 48, true);

		// finally display all rendered content
		window.display();
	}

	void SceneShaderGallery::do_action([[maybe_unused]] const Engine::Systems::Action& action)
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
				print_current_shader_name();
			}
			if(action.name() == "Next")
			{
				selection_++;
				print_current_shader_name();
			}
			if(action.name() == "Quit")
			{
				game_->quit();
			}
		}
	}

	void SceneShaderGallery::on_end() {}

}  // namespace Engine::Scene