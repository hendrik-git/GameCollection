/// @file SceneShaderGallery.hpp
/// @brief ...
/// @details ...
/// @see ...
/// @date 2022.11.26
/// @author Hendrik Poettker

#pragma once
#include "Scene.hpp"

class SceneParticleGallery : public Scene
{
  public:
	SceneParticleGallery(GameEngine* engine) : Scene(engine)
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
	int		   selection_{0};
	Vec2	   world_size_{1200, 800};
};