#include <GameScenes/Scene.hpp>

namespace Engine::Scene
{
	BaseScene::BaseScene(GameEngine* engine) : game_(engine) {}

	void BaseScene::register_action(int key, std::string name)
	{
		action_map_[key] = name;
	}

	bool BaseScene::has_ended()
	{
		return has_ended_;
	}

	size_t BaseScene::current_frame()
	{
		return current_frame_;
	}

	ActionMap& BaseScene::get_ActionMap()
	{
		return action_map_;
	}

	void BaseScene::set_paused(bool pause)
	{
		is_paused_ = pause;
	}

}  // namespace Engine::Scene