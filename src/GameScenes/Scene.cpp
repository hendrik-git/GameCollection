#include <GameScenes/Scene.hpp>

namespace Engine::Scene
{
	BaseScene::BaseScene(GameEngine* engine) : game_(engine) {}

	void BaseScene::register_action(int key, const std::string& name)
	{
		action_map_[key] = name;
	}

	auto BaseScene::has_ended() const noexcept -> bool
	{
		return has_ended_;
	}

	auto BaseScene::current_frame() const noexcept -> size_t
	{
		return current_frame_;
	}

	auto BaseScene::get_ActionMap() -> ActionMap&
	{
		return action_map_;
	}

	void BaseScene::set_paused(bool pause)
	{
		is_paused_ = pause;
	}

}  // namespace Engine::Scene