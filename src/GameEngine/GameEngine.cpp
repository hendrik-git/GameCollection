#include <GameEngine/GameEngine.hpp>
#include <fmt/core.h>
#include <random>

namespace
{
	auto calc_direction(Vec2 from, Vec2 to) -> Vec2
	{
		return (to - from);
	}

	auto get_random_start_pos(int max_x, int max_y) -> Vec2
	{
		auto rd	   = std::random_device{};
		auto mtgen = std::mt19937{rd()};
		auto x_ud  = std::uniform_int_distribution<>{0, max_x};
		auto y_ud  = std::uniform_int_distribution<>{0, max_y};
		return {static_cast<float>(x_ud(mtgen)), static_cast<float>(y_ud(mtgen))};
	}
	
	auto get_random_dir() -> Vec2
	{
		auto rd	   = std::random_device{};
		auto mtgen = std::mt19937{rd()};
		auto ud	   = std::uniform_int_distribution<>{-100, 100};
		return {Vec2{static_cast<float>(ud(mtgen)), static_cast<float>(ud(mtgen))}.normalize()};
	}

}  // namespace

#pragma region public functions

GameEngine::GameEngine(const fs::path config)
{
	// Load necessary data
	if(!font_.loadFromFile("../../data/fonts/Gidole.ttf"))
	{
		throw std::exception("Failed to load font");
	}
	init(config);
}

void GameEngine::run()
{
	while(running_)
	{
		user_input();
		if(!paused_)
		{
			spawn_entities();
			reduce_lifespan();
			manager_.update();
			movement();
			collision();
		}
		render();
		current_frame_++;
	}
}

#pragma endregion

#pragma region private functions
void GameEngine::init(const fs::path config)
{
	/// @todo read the config file

	// set up window default parameters
	window_.create(sf::VideoMode({1200, 800}), "GameCollection");
	window_.setFramerateLimit(60);

	spawn_player();
}

void GameEngine::user_input()
{
	sf::Event event;
	while(window_.pollEvent(event))
	{
		if(event.type == sf::Event::Closed)
		{
			window_.close();
			running_ = false;
		}

		if(event.type == sf::Event::KeyPressed)
		{
			switch(event.key.code)
			{
				// Movement input
				case sf::Keyboard::W:
					player_->input->up = true;
					break;
				case sf::Keyboard::A:
					player_->input->left = true;
					break;
				case sf::Keyboard::S:
					player_->input->down = true;
					break;
				case sf::Keyboard::D:
					player_->input->right = true;
					break;
				// toggle pause
				case sf::Keyboard::Escape:
					paused_ = !paused_;
				default:
					break;
			}
		}

		if(event.type == sf::Event::KeyReleased)
		{
			switch(event.key.code)
			{
				case sf::Keyboard::W:
					player_->input->up = false;
					break;
				case sf::Keyboard::A:
					player_->input->left = false;
					break;
				case sf::Keyboard::S:
					player_->input->down = false;
					break;
				case sf::Keyboard::D:
					player_->input->right = false;
					break;
				default:
					break;
			}
		}

		player_->mouse->lmb = sf::Mouse::isButtonPressed(sf::Mouse::Left);
		player_->mouse->rmb = sf::Mouse::isButtonPressed(sf::Mouse::Right);
		player_->mouse->x	= static_cast<float>(sf::Mouse::getPosition(window_).x);
		player_->mouse->y	= static_cast<float>(sf::Mouse::getPosition(window_).y);
	}
}

void GameEngine::spawn_entities()
{
	static auto bullet_cooldown{0};
	if(player_->mouse->lmb && bullet_cooldown == 0)
	{
		auto bullet		  = manager_.add_entity("bullet");
		auto mouse_pos	  = Vec2{player_->mouse->x, player_->mouse->y};
		auto direction	  = calc_direction(player_->transform->pos, mouse_pos);
		bullet->transform = std::make_shared<Transform>(
			player_->transform->pos, direction.normalize() * 10, Vec2{0.F, 0.F}, 0.F);

		ShapeInit bullet_shape;
		bullet_shape.radius	   = 4.F;
		bullet_shape.fill	   = sf::Color::White;
		bullet_shape.outline   = sf::Color::Yellow;
		bullet_shape.thickness = 1.F;
		bullet->shape		   = std::make_shared<Shape>(bullet_shape);
		bullet->lifespan	   = std::make_shared<Lifespan>(100);
		bullet_cooldown		   = 10;
	}
	bullet_cooldown = std::max(0, --bullet_cooldown);

	static auto enemy_cooldown{0};
	if(enemy_cooldown == 0 && manager_.get_entities("enemy").size() <= 10)
	{
		auto enemy		 = manager_.add_entity("enemy");
		auto enemy_pos	 = get_random_start_pos(window_.getSize().x, window_.getSize().y);
		auto dir		 = get_random_dir();
		enemy->transform = std::make_shared<Transform>(enemy_pos, dir * 3, Vec2{0.F, 0.F}, 0.F);

		ShapeInit enemy_shape;
		enemy_shape.radius	  = 40.F;
		enemy_shape.points	  = 3;
		enemy_shape.fill	  = sf::Color::Red;
		enemy_shape.outline	  = sf::Color::White;
		enemy_shape.thickness = 1.F;
		enemy->shape		  = std::make_shared<Shape>(enemy_shape);
		enemy_cooldown		  = 40;

		enemy->score = std::make_shared<Score>(10);
	}
	enemy_cooldown = std::max(0, --enemy_cooldown);
}

void GameEngine::reduce_lifespan()
{
	for(auto entity : manager_.get_entities())
	{
		if(auto lifespan_component = entity->lifespan; lifespan_component)
		{
			lifespan_component->remaining > 0 ? [=]() { lifespan_component->remaining--; }()
											  : [=]() { entity->destroy(); }();
		}
	}
}

void GameEngine::movement()
{
	// handle player movement
	if(player_->input->up)
	{
		player_->transform->pos.y -= 5.F;
	}
	if(player_->input->down)
	{
		player_->transform->pos.y += 5.F;
	}
	if(player_->input->right)
	{
		player_->transform->pos.x += 5.F;
	}
	if(player_->input->left)
	{
		player_->transform->pos.x -= 5.F;
	}

	auto x_min = 0.F;
	auto y_min = 0.F;
	auto x_max = static_cast<float>(window_.getSize().x);
	auto y_max = static_cast<float>(window_.getSize().y);

	for(auto& entity : manager_.get_entities())
	{
		if(auto transf = entity->transform; transf)
		{
			[[maybe_unused]] auto& x   = transf->pos.x;
			[[maybe_unused]] auto& dx  = transf->vel.x;
			[[maybe_unused]] auto& ddx = transf->acc.x;
			[[maybe_unused]] auto& y   = transf->pos.y;
			[[maybe_unused]] auto& dy  = transf->vel.y;
			[[maybe_unused]] auto& ddy = transf->acc.y;

			x += dx;
			y += dy;

			x = std::clamp(x, x_min, x_max);
			y = std::clamp(y, y_min, y_max);

			transf->angle += 2.F;
		}
	}

	for(auto& bullet : manager_.get_entities("bullet"))
	{
		auto bullet_x = bullet->transform->pos.x;
		auto bullet_y = bullet->transform->pos.y;

		if(bullet_x <= x_min || bullet_x >= x_max || bullet_y <= y_min || bullet_y >= y_max)
		{
			bullet->destroy();
		}
	}

	// have enemies bounce from the borders of the screen
	for(auto& enemy : manager_.get_entities("enemy"))
	{
		auto enemy_x = enemy->transform->pos.x;
		auto enemy_y = enemy->transform->pos.y;

		if(enemy_x <= x_min || enemy_x >= x_max)
		{
			enemy->transform->vel.x *= -1;
		}
		if(enemy_y <= y_min || enemy_y >= y_max)
		{
			enemy->transform->vel.y *= -1;
		}
	}
}

void GameEngine::collision()
{
	// collision bullet <-> enemy
	for(auto& bullet : manager_.get_entities("bullet"))
	{
		assert(bullet->shape && "Bullet has no shape component");
		assert(bullet->transform && "Bullet has no tranform component");

		for(auto& enemy : manager_.get_entities("enemy"))
		{
			assert(enemy->shape && "Enemy has no shape component");
			assert(enemy->transform && "Enemy has no tranform component");

			// two circles collide, when their distance is less than their radii
			auto bullet_r	 = bullet->shape->circle.getRadius();
			auto enemy_r	 = enemy->shape->circle.getRadius();
			auto radius_sq	 = (bullet_r + enemy_r) * (bullet_r + enemy_r);
			auto distance_sq = get_distance_sq(bullet->transform->pos, enemy->transform->pos);

			if(distance_sq <= radius_sq)
			{
				bullet->destroy();
				enemy->destroy();
				score_ += enemy->score->score;
			}
		}
	}
}

void GameEngine::render()
{
	window_.clear();

	for(auto entity : manager_.get_entities())
	{
		if(auto shape = entity->shape; shape)
		{
			// assume every shape has also a transform component
			assert(entity->transform && "Missing transform component");

			auto& circle = shape->circle;
			circle.setPosition({entity->transform->pos.x, entity->transform->pos.y});
			circle.setRotation(sf::degrees(entity->transform->angle));

			window_.draw(shape->circle);
		}
	}

	// draw HUD elements on top
	sf::Text text;
	text.setFont(font_);
	text.setString(fmt::format("Health {}\nScore  {:>04}", "INF", score_));
	text.setCharacterSize(24);	// in pixels
	text.setFillColor(sf::Color::White);
	text.setStyle(sf::Text::Bold);
	window_.draw(text);

	window_.display();
}

void GameEngine::spawn_player()
{
	player_ = manager_.add_entity("player");
	player_->transform =
		std::make_shared<Transform>(Vec2{400.F, 400.F}, Vec2{0.F, 0.F}, Vec2{0.F, 0.F}, 0.F);

	ShapeInit player_shape;
	player_shape.radius	   = 32.F;
	player_shape.points	   = 8;
	player_shape.fill	   = sf::Color::Yellow;
	player_shape.outline   = sf::Color::White;
	player_shape.thickness = 4.F;
	player_->shape		   = std::make_shared<Shape>(player_shape);

	player_->input = std::make_shared<Input>();
	player_->mouse = std::make_shared<Mouse>();
}

#pragma endregion