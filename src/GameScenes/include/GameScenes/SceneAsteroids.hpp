/// @file SceneAsteroids.hpp
/// @brief ...
/// @details ...
/// @see ...
/// @date 2022.11.16
/// @author Hendrik Poettker

#pragma once
#include "Scene.hpp"

namespace Engine::Scene
{
	class SceneAsteroids : public BaseScene
	{
	  public:
		SceneAsteroids(GameEngine* engine) : BaseScene(engine)
		{
			init();
		};

		void update() override;
		void render() override;
		void do_action(const Engine::Systems::Action& action) override;
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
		bool	   game_over_	  = false;
	};
}  // namespace Engine::Scene