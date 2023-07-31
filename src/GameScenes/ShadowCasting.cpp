#include <GameEngine/GameEngine.hpp>
#include <GameScenes/ShadowCasting.hpp>

namespace Engine::Scene
{
	void ShadowCasting::toggle_tile()
	{
	}

	void ShadowCasting::init()
	{
		register_action(sf::Keyboard::Escape, "Quit");
		register_action(sf::Mouse::Left, "MouseL");
		register_action(sf::Mouse::Right, "MouseR");
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
		window.clear();

		// draw all entities
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