/// @file SceneAsteroids.hpp
/// @brief ...
/// @details ...
/// @see ...
/// @date 2022.11.16
/// @author Hendrik Poettker

#pragma once
#include "Scene.hpp"

class SceneAsteroids : public Scene
{
  public:
	SceneAsteroids(GameEngine* engine) : Scene(engine)
	{
		init();
	};

	void update() override;
	void render() override;
	void do_action(const Action& action) override;
	void on_end() override;

	void init();
	void spawn_entities();
	void spawn_player();
	void reduce_lifespan();

	void movement();
	void collision();

	sf::Text   text_;
	sf::Sprite background_;
	int		   score_		  = 0;
	Vec2	   world_size_	  = {2000, 2000};
	bool	   draw_hitboxes_ = false;
	bool	   draw_sprites_  = true;
};