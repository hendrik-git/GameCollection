#include <GameEngine/GameEngine.hpp>
#include <GameScenes/Template.hpp>

namespace Engine::Scene
{
	void Template::init()
	{
		register_action(sf::Keyboard::Escape, "Quit");
	}

	void Template::update()
	{
		if(!is_paused_)
		{
			// todo
		}
		current_frame_++;
	}

	void Template::render()
	{
		using namespace Engine::Components;

		auto& window = game_->window();
		auto  view	 = window.getDefaultView();

		// from now on, fill the blank canvas
		window.clear();

		// draw all entities
	}

	void Template::do_action(const Engine::Systems::Action& action)
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
	
	void Template::on_end() {}
}  // namespace Engine::Scene