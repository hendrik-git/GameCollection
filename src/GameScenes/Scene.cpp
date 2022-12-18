#include <GameScenes/Scene.hpp>

namespace Engine::Scene
{
	BaseScene::BaseScene(GameEngine* engine) : game_(engine) {}

	// void Scene::simulate() {}

	void BaseScene::register_action(int key, std::string name)
	{
		action_map_[key] = name;
	}

	// size_t Scene::width()
	//{
	//	return size_t();
	//}
	//
	// size_t Scene::height()
	//{
	//	return size_t();
	//}

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

	// void Scene::draw_line(Vec2 p1, Vec2 p2) {}

	void BaseScene::set_paused(bool pause)
	{
		is_paused_ = pause;
	}

}  // namespace Engine::Scene