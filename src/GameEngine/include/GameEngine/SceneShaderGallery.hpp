/// @file SceneShaderGallery.hpp
/// @brief ...
/// @details ...
/// @see ...
/// @date 2022.11.26
/// @author Hendrik Poettker

#pragma once
#include "Scene.hpp"

class SceneShaderGallery : public Scene
{
  public:
	SceneShaderGallery(GameEngine* engine) : Scene(engine)
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