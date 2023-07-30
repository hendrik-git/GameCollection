/// @file Template.hpp
/// @brief ...
/// @details ...
/// @see ...
/// @date 2023.07.31
/// @author Hendrik Poettker

#pragma once
#include "Scene.hpp"

namespace Engine::Scene
{
	class Template : public BaseScene
	{
	  public:
		explicit Template(GameEngine* engine) : BaseScene(engine)
		{
			init();
		};

		void init();
		void update() override;
		void render() override;
		void do_action(const Engine::Systems::Action& action) override;
		void on_end() override;

	  private:
	};
}  // namespace Engine::Scene