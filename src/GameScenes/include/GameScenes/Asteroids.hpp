/// @file SceneAsteroids.hpp
/// @brief ...
/// @details ...
/// @see ...
/// @date 2022.11.16
/// @author Hendrik Poettker

#pragma once
#include "Scene.hpp"
#include <GameEngine/ParticleManager.hpp>
#include <GameSystems/Score.hpp>

namespace Engine::Scene
{
	class Asteroids : public BaseScene
	{
	  public:
		explicit Asteroids(GameEngine* engine);

		void update() override;
		void render() override;
		void do_action(const Engine::Systems::Action& action) override;
		void on_end() override;

		void init();
		void spawn_entities();
		void spawn_planet();
		void spawn_player();
		void reduce_lifespan();

		void movement();
		void collision();

		void reset();

	  private:
		sf::Text		   text_;
		sf::Sprite		   background_;
		ParticleManager	   particles_;
		Vec2			   world_size_	  = {2000, 2000};
		bool			   draw_hitboxes_ = false;
		bool			   draw_sprites_  = true;
		bool			   game_over_	  = false;
		GameSystems::Score score_;
		entt::entity	   player2_;
	};
}  // namespace Engine::Scene