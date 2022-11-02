#include <GameEngine/Scene.hpp>

Scene::Scene(GameEngine* engine) : game_(engine) {}

// void Scene::simulate() {}

void Scene::register_action(int key, std::string name) 
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

bool Scene::has_ended()
{
	return has_ended_;
}

size_t Scene::current_frame()
{
	return current_frame_;
}

ActionMap& Scene::get_ActionMap()
{
	return action_map_;
}

// void Scene::draw_line(Vec2 p1, Vec2 p2) {}

void Scene::set_paused(bool pause)
{
	is_paused_ = pause;
}
