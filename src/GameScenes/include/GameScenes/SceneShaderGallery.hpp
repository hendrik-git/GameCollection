/// @file SceneShaderGallery.hpp
/// @brief ...
/// @details ...
/// @see ...
/// @date 2022.11.26
/// @author Hendrik Poettker

#pragma once
#include "Scene.hpp"

namespace Engine::Scene
{
	class SceneShaderGallery : public BaseScene
	{
	  public:
		SceneShaderGallery(GameEngine* engine) : BaseScene(engine)
		{
			init();
		};

		void init();
		void update() override;
		void render() override;
		void do_action(const Action& action) override;
		void on_end() override;

		void spawn_player();

		sf::Text   text_;
		sf::Sprite background_;
		int		   selection_{0};
		Vec2	   world_size_{1200, 800};
	};
}  // namespace Engine::Scene