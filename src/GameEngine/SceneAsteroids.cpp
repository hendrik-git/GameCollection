#include "GameEngine/SceneAsteroids.hpp"
#include "GameEngine/GameEngine.hpp"
#include <fmt/core.h>
#include <random>

namespace
{
	auto calc_direction(Vec2 from, Vec2 to) -> Vec2
	{
		return (to - from);
	}

	auto get_random_int(int from, int to)
	{
		static auto rd	  = std::random_device{};
		static auto mtgen = std::mt19937{rd()};
		auto		ud	  = std::uniform_int_distribution<>{from, to};
		return ud(mtgen);
	}

	auto get_random_start_pos(int max_x, int max_y) -> Vec2
	{
		return {static_cast<float>(get_random_int(0, max_x)),
				static_cast<float>(get_random_int(0, max_y))};
	}

	auto get_random_dir() -> Vec2
	{
		return {Vec2{static_cast<float>(get_random_int(-100, 100)),
					 static_cast<float>(get_random_int(-100, 100))}
					.normalize()};
	}
}  // namespace

void SceneAsteroids::spawn_entities()
{
	auto& pos	= player_->get_component<Transform>().pos;
	auto& mouse = player_->get_component<Mouse>();

	static auto bullet_cooldown{0};
	if(player_->get_component<Input>().space && bullet_cooldown == 0)
	{
		auto bullet	   = entities_.add_entity("bullet");
		auto mouse_pos = Vec2{mouse.x, mouse.y};
		auto angle	   = player_->get_component<Transform>().angle;
		auto direction = direction_from_degree(angle);
		// auto direction = calc_direction(pos, mouse_pos);
		bullet->add_component<Transform>(pos + (direction * 20), direction * 25, angle + 90.F);

		ShapeInit bullet_shape;
		bullet_shape.radius	   = 4.F;
		bullet_shape.fill	   = sf::Color::White;
		bullet_shape.outline   = sf::Color::Yellow;
		bullet_shape.thickness = 1.F;
		bullet->add_component<Shape>(bullet_shape);
		bullet->add_component<Lifespan>(120);
		bullet->add_component<Drawable>("Laser", game_->assets().get_texture("LaserShot"));

		bullet_cooldown = 10;
	}
	bullet_cooldown = std::max(0, --bullet_cooldown);

	static auto enemy_cooldown{0};
	if(enemy_cooldown == 0 && entities_.get_entities("enemy").size() <= 15)
	{
		Vec2 enemy_pos{0.F, 0.F};
		do
		{
			enemy_pos = get_random_start_pos(static_cast<int>(world_size_.x),
											 static_cast<int>(world_size_.y));
		} while(get_distance_sq(enemy_pos, pos) < 30 * 30);

		auto enemy = entities_.add_entity("enemy");
		auto dir   = get_random_dir();
		enemy->add_component<Transform>(enemy_pos, dir * 3);

		ShapeInit enemy_shape;
		enemy_shape.radius	  = 40.F;
		enemy_shape.points	  = 3;
		enemy_shape.fill	  = sf::Color::Red;
		enemy_shape.outline	  = sf::Color::White;
		enemy_shape.thickness = 1.F;
		enemy->add_component<Shape>(enemy_shape);
		enemy_cooldown = 40;

		enemy->add_component<Score>(10);
	}
	enemy_cooldown = std::max(0, --enemy_cooldown);
}

void SceneAsteroids::spawn_player()
{
	player_ = entities_.add_entity("player");
	player_->add_component<Transform>(Vec2{world_size_.x / 2, world_size_.y / 2});
	ShapeInit player_shape;
	player_shape.radius	   = 12.F;
	player_shape.points	   = 32;
	player_shape.fill	   = sf::Color::Yellow;
	player_shape.outline   = sf::Color::White;
	player_shape.thickness = 4.F;
	// player_->add_component<Shape>(player_shape);
	player_->add_component<Input>();
	player_->add_component<Mouse>();

	auto& draw =
		player_->add_component<Drawable>("PlayerShip", game_->assets().get_texture("PlayerShip"));
	draw.set_rotation(90.F);
}

void SceneAsteroids::reduce_lifespan()
{
	for(auto& entity : entities_.get_entities())
	{
		if(auto& lifespan = entity->get_component<Lifespan>(); lifespan.has)
		{
			lifespan.remaining > 0 ? [&]() { lifespan.remaining--; }()
								   : [=]() { entity->destroy(); }();
		}
	}
}

void SceneAsteroids::movement()
{
	// handle player movement
	auto& player_pos   = player_->get_component<Transform>().pos;
	auto& player_angle = player_->get_component<Transform>().angle;

	if(player_->get_component<Input>().up)
	{
		player_pos += direction_from_degree(player_angle) * 5.F;
	}
	if(player_->get_component<Input>().down)
	{
		player_pos -= direction_from_degree(player_angle) * 5.F;
	}
	if(player_->get_component<Input>().right)
	{
		player_angle += 5.F;
	}
	if(player_->get_component<Input>().left)
	{
		player_angle -= 5.F;
	}

	auto x_min = 0.F;
	auto y_min = 0.F;
	auto x_max = world_size_.x;
	auto y_max = world_size_.y;

	for(auto& entity : entities_.get_entities())
	{
		if(auto& transf = entity->get_component<Transform>(); transf.has)
		{
			transf.prev_pos = transf.pos;

			[[maybe_unused]] auto& x  = transf.pos.x;
			[[maybe_unused]] auto& dx = transf.vel.x;
			[[maybe_unused]] auto& y  = transf.pos.y;
			[[maybe_unused]] auto& dy = transf.vel.y;

			x = std::clamp(x + dx, x_min, x_max);
			y = std::clamp(y + dy, y_min, y_max);
		}
	}

	for(auto& bullet : entities_.get_entities("bullet"))
	{
		auto bullet_x = bullet->get_component<Transform>().pos.x;
		auto bullet_y = bullet->get_component<Transform>().pos.y;

		if(bullet_x <= x_min || bullet_x >= x_max || bullet_y <= y_min || bullet_y >= y_max)
		{
			bullet->destroy();
		}
	}

	// have enemies bounce from the borders of the screen
	for(auto& enemy : entities_.get_entities("enemy"))
	{
		auto& transf  = enemy->get_component<Transform>();
		auto& enemy_x = transf.pos.x;
		auto& enemy_y = transf.pos.y;

		if(enemy_x <= x_min || enemy_x >= x_max)
		{
			transf.vel.x *= -1;
		}
		if(enemy_y <= y_min || enemy_y >= y_max)
		{
			transf.vel.y *= -1;
		}

		transf.angle += 2.F;
	}
}

void SceneAsteroids::init()
{
	register_action(sf::Keyboard::Up, "Up");
	register_action(sf::Keyboard::Left, "Left");
	register_action(sf::Keyboard::Down, "Down");
	register_action(sf::Keyboard::Right, "Right");
	register_action(sf::Keyboard::P, "Pause");
	register_action(sf::Keyboard::Space, "Shoot");
	register_action(sf::Keyboard::Escape, "Quit");

	auto& texture = game_->assets().get_texture("Background");
	texture.setRepeated(true);
	background_.setPosition({0, 0});
	background_.setTexture(texture);
	background_.setTextureRect(
		sf::IntRect{{0, 0}, {static_cast<int>(world_size_.x), static_cast<int>(world_size_.y)}});

	spawn_player();
}

void SceneAsteroids::collision()
{
	// collision bullet <-> enemy
	for(auto& bullet : entities_.get_entities("bullet"))
	{
		assert(bullet->get_component<Transform>().has && "Bullet has no tranform component");

		for(auto& enemy : entities_.get_entities("enemy"))
		{
			assert(enemy->get_component<Shape>().has && "Enemy has no shape component");
			assert(enemy->get_component<Transform>().has && "Enemy has no tranform component");

			// two circles collide, when their distance is less than their radii
			auto bullet_r	 = bullet->get_component<Shape>().circle.getRadius();
			auto enemy_r	 = enemy->get_component<Shape>().circle.getRadius();
			auto radius_sq	 = (bullet_r + enemy_r) * (bullet_r + enemy_r);
			auto distance_sq = get_distance_sq(bullet->get_component<Transform>().pos,
											   enemy->get_component<Transform>().pos);

			if(distance_sq <= radius_sq)
			{
				bullet->destroy();
				enemy->destroy();
				score_ += enemy->get_component<Score>().score;
			}
		}
	}
}

void SceneAsteroids::update()
{
	// user_input();
	if(!is_paused_)
	{
		spawn_entities();
		reduce_lifespan();
		entities_.update();
		movement();
		collision();
	}
	render();
	current_frame_++;
}

void SceneAsteroids::render()
{
	auto& window = game_->window();
	auto  view	 = window.getDefaultView();
	window.clear();

	// draw background
	window.draw(background_);

	// draw game entities
	for(auto& entity : entities_.get_entities())
	{
		if(auto& shape = entity->get_component<Shape>(); shape.has)
		{
			// assume every shape has also a transform component
			assert(entity->get_component<Transform>().has && "Missing transform component");

			auto& circle = shape.circle;
			auto& transf = entity->get_component<Transform>();

			circle.setPosition({transf.pos.x, transf.pos.y});
			circle.setRotation(sf::degrees(transf.angle));

			window.draw(shape.circle);
		}

		if(auto& shape = entity->get_component<Drawable>(); shape.has)
		{
			// assume every shape has also a transform component
			assert(entity->get_component<Transform>().has && "Missing transform component");
			// shape.update();
			auto  sprite = shape.get_sprite();
			auto& transf = entity->get_component<Transform>();

			sprite.setPosition({transf.pos.x, transf.pos.y});
			sprite.setRotation(sf::degrees(transf.angle + shape.get_rotation()));

			window.draw(sprite);
		}
	}

	// auto& ship_texture = game_->assets().get_texture("PlayerShip");
	// auto  ship		   = sf::Sprite{ship_texture};
	auto transf = player_->get_component<Transform>();

	// ship.setOrigin((sf::Vector2f)ship_texture.getSize() / 2.f);
	// ship.setPosition({transf.pos.x, transf.pos.y});
	// ship.setRotation(sf::degrees(transf.angle + 90));
	// ship.setColor(sf::Color{255, 255, 255, 50});
	// window.draw(ship);

	const auto view_size  = view.getSize();
	const auto view_min_x = view_size.x / 2;
	const auto view_min_y = view_size.y / 2;
	const auto view_max_x = world_size_.x - view_min_x;
	const auto view_max_y = world_size_.y - view_min_y;

	auto view_point = sf::Vector2f{std::clamp(transf.pos.x, view_min_x, view_max_x),
								   std::clamp(transf.pos.y, view_min_y, view_max_y)};
	view.setCenter(view_point);

	// draw HUD elements on top
	sf::Text text;
	text.setFont(game_->assets().get_font("Gidole"));
	text.setString(fmt::format("Score {:>04}", score_));
	text.setCharacterSize(24);	// in pixels
	text.setFillColor(sf::Color::White);
	text.setStyle(sf::Text::Bold);
	text.setPosition(window.mapPixelToCoords({10, 10}, view));
	window.draw(text);


	window.setView(view);
	window.display();
}

void SceneAsteroids::do_action(const Action& action)
{
	if(action.type() == "Start")
	{
		if(action.name() == "Up")
		{
			player_->get_component<Input>().up = true;
		}
		if(action.name() == "Down")
		{
			player_->get_component<Input>().down = true;
		}
		if(action.name() == "Left")
		{
			player_->get_component<Input>().left = true;
		}
		if(action.name() == "Right")
		{
			player_->get_component<Input>().right = true;
		}
		if(action.name() == "Shoot")
		{
			player_->get_component<Input>().space = true;
		}
		if(action.name() == "Pause")
		{
			set_paused(!is_paused_);
		}
		if(action.name() == "Quit")
		{
			game_->quit();
		}
	}
	else if(action.type() == "End")
	{
		if(action.name() == "Up")
		{
			player_->get_component<Input>().up = false;
		}
		if(action.name() == "Down")
		{
			player_->get_component<Input>().down = false;
		}
		if(action.name() == "Left")
		{
			player_->get_component<Input>().left = false;
		}
		if(action.name() == "Right")
		{
			player_->get_component<Input>().right = false;
		}
		if(action.name() == "Shoot")
		{
			player_->get_component<Input>().space = false;
		}
	}
}

void SceneAsteroids::on_end() {}
