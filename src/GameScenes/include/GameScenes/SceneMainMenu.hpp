/// @file SceneMainMenu.hpp
/// @brief ...
/// @details ...
/// @see ...
/// @date 2022.11.16
/// @author Hendrik Poettker

#pragma once
#include "Scene.hpp"

namespace Engine::Scene
{
	class SceneMainMenu : public BaseScene
	{
	  public:
		SceneMainMenu(GameEngine* engine) : BaseScene(engine)
		{
			init();
		};

		void init();
		void update() override;
		void render() override;
		void do_action(const Action& action) override;
		void on_end() override;

		sf::Text   text_;
		sf::Sprite background_;
		int		   selection_;
	};
}  // namespace Engine::Scene