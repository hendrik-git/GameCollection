/// @file SceneMainMenu.hpp
/// @brief ...
/// @details ...
/// @see ...
/// @date 2022.11.16
/// @author Hendrik Pöttker

#pragma once
#include "Scene.hpp"

class SceneMainMenu : public Scene
{
  public:
	SceneMainMenu(GameEngine* engine) : Scene(engine)
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