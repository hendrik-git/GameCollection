/// @file ShadowCasting.hpp
/// @brief ...
/// @details ...
/// @see ...
/// @date 2023.07.31
/// @author Hendrik Poettker

#pragma once
#include "Scene.hpp"
#include <array>

namespace Engine::Scene
{
	using EdgesArray = std::array<entt::entity, 4>;

	struct Cell
	{
		int		   pos_x;
		int		   pos_y;
		bool	   exists = false;
		EdgesArray edges;  //!< contains references to the edges
	};

	using World = std::array<Cell, 80 * 60>;

	struct Edge
	{
		int start_x;
		int start_y;
		int end_x;
		int end_y;
	};

	class ShadowCasting : public BaseScene
	{
	  public:
		explicit ShadowCasting(GameEngine* engine) : BaseScene(engine)
		{
			init();
		};

		void init();
		void update() override;
		void render() override;
		void do_action(const Engine::Systems::Action& action) override;
		void on_end() override;

	  private:
		void toggle_tile();

		const int worldwidth_  = 40;
		const int worldheight_ = 30;
		const int tile_size_   = 20;
		World	  tiles_;
	};
}  // namespace Engine::Scene