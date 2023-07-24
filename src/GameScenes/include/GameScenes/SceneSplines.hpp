/// @file SceneAsteroids.hpp
/// @brief ...
/// @details ...
/// @see ...
/// @date 2022.11.16
/// @author Hendrik Poettker

#pragma once
#include "Scene.hpp"
#include <GameMath/Spline.hpp>

namespace Engine::Scene
{
	class SceneSplines : public BaseScene
	{
	  public:
		SceneSplines([[maybe_unused]] GameEngine* engine) : BaseScene(engine)
		{
			init();
		};
		void update() override;
		void render() override;
		void do_action(const Systems::Action& action) override;
		void on_end() override{};

		void init();

	  private:
		sf::Text   text_;
		sf::Sprite background_;
		bool	   paused_	   = false;
		Vec2	   world_size_ = {2000, 2000};

		Math::Spline spline_;
		unsigned	 selected_point_ = 0;
		struct Movement
		{
			bool up	   = false;
			bool down  = false;
			bool left  = false;
			bool right = false;
		} movement_;

		float position_ = 0.f;
	};
}  // namespace Engine::Scene