#include <GameEngine/GameEngine.hpp>
#include <GameScenes/ShadowCasting.hpp>
#include <cassert>

namespace Engine::Scene
{
	void ShadowCasting::toggle_tile()
	{
		auto [x, y] = sf::Mouse::getPosition(game_->window());
		int i		= (y / tile_size_) * worldwidth_ + x / tile_size_;
		std::cout << fmt::format("i == (y / tile_size_) * worldheight_ + x / tile_size_\n");
		std::cout << fmt::format(
			"{} == {} * worldheight_ + {} / {}\n", i, y / tile_size_, x, tile_size_);

		assert(i >= 0 && i < worldheight_ * worldwidth_ && "Invalid index calculated");

		tiles_.at(i).exists = !tiles_.at(i).exists;
		std::cout << fmt::format("Settings {} to {}\n", i, tiles_.at(i).exists);
	}

	void ShadowCasting::init()
	{
		register_action(sf::Keyboard::Escape, "Quit");
		register_action(sf::Mouse::Left, "MouseL");
		register_action(sf::Mouse::Right, "MouseR");


		auto  shape1 = registry_.create();
		auto& shape	 = registry_.emplace<sf::ConvexShape>(shape1);
		shape.setPointCount(5);
		shape.setPoint(0, sf::Vector2f(0, 0));
		shape.setPoint(1, sf::Vector2f(150, 10));
		shape.setPoint(2, sf::Vector2f(120, 90));
		shape.setPoint(3, sf::Vector2f(30, 100));
		shape.setPoint(4, sf::Vector2f(0, 50));
	}

	void ShadowCasting::update()
	{
		if(!is_paused_)
		{
			// todo
		}
		current_frame_++;
	}

	void ShadowCasting::render()
	{
		using namespace Engine::Components;

		auto& window = game_->window();
		auto  view	 = window.getDefaultView();

		// from now on, fill the blank canvas
		// std::cout << "Render called\n";
		window.clear(sf::Color{125, 125, 125, 125});

		// draw all entities
		for(auto x = 0; x < worldwidth_; ++x)
		{
			for(auto y = 0; y < worldheight_; ++y)
			{
				// std::cout << fmt::format("Drawing {}|{}\n", x, y);
				auto			   i = y * worldwidth_ + x;
				sf::RectangleShape rectangle({(float)tile_size_, (float)tile_size_});
				rectangle.setPosition({(float)x * tile_size_, (float)y * tile_size_});
				rectangle.setFillColor(tiles_.at(i).exists ? sf::Color::Red : sf::Color::Black);
				window.draw(rectangle);
			}
		}

		for(auto entity : registry_.view<sf::ConvexShape>())
		{
			window.draw(registry_.get<sf::ConvexShape>(entity));
		}

		// finally display all rendered content
		window.display();
	}
	void ShadowCasting::do_action(const Engine::Systems::Action& action)
	{
		using namespace Engine::Components;

#define MAP_STRING_TO(string, code) \
	if(action.name() == string)     \
	{                               \
		code                        \
	}

		try
		{
			if(action.type() == Engine::Systems::ActionType::Start)
			{
				MAP_STRING_TO("Quit", game_->change_scene("MainMenu"); return;)
				MAP_STRING_TO("MouseL", toggle_tile();)
				MAP_STRING_TO("MouseR", std::cout << "MouseRight\n";)
			}
			else if(action.type() == Engine::Systems::ActionType::End)
			{
			}
		}
		catch(const std::exception& e)
		{
			std::cerr << fmt::format("Exception caught {} in {}\n", e.what(), __FILE__);
		}
#undef MAP_STRING_TO
	}
	void ShadowCasting::on_end() {}
}  // namespace Engine::Scene