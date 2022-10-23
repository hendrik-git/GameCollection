#include <GameEngine/GameEngine.hpp>

namespace
{
}

#pragma region public functions

GameEngine::GameEngine(const fs::path config)
{
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
	if(player_->mouse->lmb)
	{
		auto bullet		  = manager_.add_entity("bullet");
		bullet->transform = std::make_shared<Transform>(
			Vec2{player_->mouse->x, player_->mouse->y}, Vec2{1.F, 1.F}, Vec2{0.F, 0.F}, 0.F);

		ShapeInit bullet_shape;
		bullet->shape	 = std::make_shared<Shape>(bullet_shape);
		bullet->lifespan = std::make_shared<Lifespan>(100);
	}
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

			x = std::clamp(x, 0.F, static_cast<float>(window_.getSize().x));
			y = std::clamp(y, 0.F, static_cast<float>(window_.getSize().y));

			transf->angle += 2.F;
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
	player_->shape = std::make_shared<Shape>(player_shape);

	player_->input = std::make_shared<Input>();
	player_->mouse = std::make_shared<Mouse>();
}

#pragma endregion