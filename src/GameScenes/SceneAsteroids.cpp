#include <CodeHelpers/Overload.hpp>
#include <GameEngine/Collision.hpp>
#include <GameEngine/GameEngine.hpp>
#include <GameEngine/Utility.hpp>
#include <GameScenes/SceneAsteroids.hpp>
#include <format>
#include <random>

class Vec2;

namespace Engine::Scene
{
	/// @brief Anonymous namespace for SceneAsteroids.cpp
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

		/// @brief Moves and rotates the player according to input
		/// @param player pointer to the player entity (space ship)
		void move_player(PlayerPtr player)
		{
			using namespace Engine::Components;

			auto& player_pos   = player->get_component<Transform>().pos;
			auto& player_angle = player->get_component<Transform>().angle;

			if(player->get_component<Input>().up)
			{
				player_pos += direction_from_degree(player_angle) * 4.F;
			}
			if(player->get_component<Input>().down)
			{
				player_pos -= direction_from_degree(player_angle) * 4.F;
			}
			if(player->get_component<Input>().right)
			{
				player_angle += 2.F;
			}
			if(player->get_component<Input>().left)
			{
				player_angle -= 2.F;
			}
		}

	}  // namespace

	using namespace CodeHelper;

	void SceneAsteroids::spawn_entities()
	{
		using namespace Engine::Components;

		auto& pos	= player_->get_component<Transform>().pos;
		auto& mouse = player_->get_component<Mouse>();

		static auto bullet_cooldown{0};
		if(player_->get_component<Input>().space && bullet_cooldown == 0)
		{
			auto bullet	   = entities_.add_entity("bullet");
			auto mouse_pos = Vec2{mouse.x, mouse.y};
			auto angle	   = player_->get_component<Transform>().angle;
			auto direction = direction_from_degree(angle);
			bullet->add_component<Transform>(pos + (direction * 20), direction * 25, angle + 90.F);

			const auto radius = 4.F;
			auto	   circle = sf::CircleShape(radius);
			circle.setOrigin({radius, radius});
			bullet->add_component<Collision>(circle);
			bullet->add_component<Lifespan>(120);
			bullet->add_component<Drawable>("Laser", game_->assets().get_texture("Laser"));

			auto& sound = bullet->add_component<Sound>().sound;
			sound.setBuffer(game_->assets().get_sound("sfx_laser1"));
			sound.play();

			bullet_cooldown = 10;
		}
		bullet_cooldown = std::max(0, --bullet_cooldown);

		static auto enemy_cooldown{0};
		if(enemy_cooldown == 0 && entities_.get_entities("enemy").size() <= 15)
		{
			const auto nr = get_random_int(0, 9);

			Vec2 enemy_pos{0.F, 0.F};
			do
			{
				enemy_pos = get_random_start_pos(static_cast<int>(world_size_.x),
												 static_cast<int>(world_size_.y));
			} while(get_distance_sq(enemy_pos, pos) < 100 * 100);

			auto dir   = get_random_dir();
			auto enemy = entities_.add_entity("enemy");
			enemy->add_component<Transform>(enemy_pos, dir * 2.F);

			switch(nr)
			{
				default:
					[[fallthrough]];
				case 0:
					[[fallthrough]];
				case 1:
					enemy->add_component<Score>(5);
					enemy->add_component<Hitpoints>(1);
					break;
				case 2:
					[[fallthrough]];
				case 3:
					enemy->add_component<Score>(10);
					enemy->add_component<Hitpoints>(1);
					break;
				case 4:
					[[fallthrough]];
				case 5:
					enemy->add_component<Score>(20);
					enemy->add_component<Hitpoints>(2);
					break;
				case 6:
					[[fallthrough]];
				case 7:
					enemy->add_component<Score>(20);
					enemy->add_component<Hitpoints>(2);
					break;
				case 8:
					[[fallthrough]];
				case 9:
					enemy->add_component<Score>(50);
					enemy->add_component<Hitpoints>(5);
					break;
			}

			auto& texture = game_->assets().get_texture("Meteor" + std::to_string(nr));
			enemy->add_component<Collision>(Utility::get_circle_from(texture));
			enemy->add_component<Drawable>("Meteor", texture);

			enemy_cooldown = 30;
		}
		enemy_cooldown = std::max(0, --enemy_cooldown);
	}

	void SceneAsteroids::spawn_planet()
	{
		using namespace Engine::Components;
		auto& texture = game_->assets().get_texture("Planet");
		auto planet = entities_.add_entity("Planet");
		planet->add_component<Drawable>("Planet", texture).set_rotation(get_random_int(0,360));
		planet->add_component<Transform>(Vec2{world_size_.x / 2, world_size_.y / 2});
	}

	void SceneAsteroids::spawn_player()
	{
		using namespace Engine::Components;

		player_ = entities_.add_entity("player");

		auto& texture = game_->assets().get_texture("PlayerShip");
		player_->add_component<Drawable>("PlayerShip", texture).set_rotation(90.F);
		player_->add_component<Collision>(Utility::get_circle_from(texture));
		player_->add_component<Transform>(Vec2{world_size_.x / 2, world_size_.y / 2});
		player_->add_component<Hitpoints>(3);
		player_->add_component<Input>();
	}

	void SceneAsteroids::reduce_lifespan()
	{
		using namespace Engine::Components;

		for(auto& entity : entities_.get_entities())
		{
			if(auto& lifespan = entity->get_component<Lifespan>(); lifespan)
			{
				lifespan.remaining > 0 ? [&]() { lifespan.remaining--; }()
									   : [=]() { entity->destroy(); }();
			}
		}
	}

	void SceneAsteroids::movement()
	{
		using namespace Engine::Components;

		move_player(player_);

		auto x_min = 0.F;
		auto y_min = 0.F;
		auto x_max = world_size_.x;
		auto y_max = world_size_.y;

		for(auto& entity : entities_.get_entities())
		{
			if(auto& transf = entity->get_component<Transform>(); transf)
			{
				transf.prev_pos = transf.pos;

				auto& x	 = transf.pos.x;
				auto& dx = transf.vel.x;
				auto& y	 = transf.pos.y;
				auto& dy = transf.vel.y;

				x = std::clamp(x + dx, x_min, x_max);
				y = std::clamp(y + dy, y_min, y_max);

				// update the position inside the collision to match the entitys position
				if(auto& hitbox = entity->get_component<Collision>(); hitbox)
				{
					std::visit(Overload{[x, y](sf::RectangleShape& shape) {
											shape.setPosition({x, y});
										},
										[x, y](sf::CircleShape& shape) {
											shape.setPosition({x, y});
										}},
							   hitbox.shape);
				}
			}
		}

		// destroy bullets that leave the play space
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
		register_action(sf::Keyboard::G, "ToggleSprites");
		register_action(sf::Keyboard::H, "ToggleHitboxes");
		register_action(sf::Keyboard::Space, "Shoot");
		register_action(sf::Keyboard::Escape, "Quit");
		register_action(sf::Keyboard::Enter, "Reset");

		auto& texture = game_->assets().get_texture("Background");
		texture.setRepeated(true);
		background_.setPosition({0, 0});
		background_.setTexture(texture);
		background_.setTextureRect(sf::IntRect{
			{0, 0}, {static_cast<int>(world_size_.x), static_cast<int>(world_size_.y)}});

		spawn_planet();

		spawn_player();
	}

	void SceneAsteroids::collision()
	{
		using namespace Engine::Components;

		// collision bullet <-> enemy
		for(auto& bullet : entities_.get_entities("bullet"))
		{
			assert(bullet->get_component<Collision>() && "Bullet has no Collision component");

			for(auto& enemy : entities_.get_entities("enemy"))
			{
				assert(enemy->get_component<Collision>() && "Enemy has no Collision component");

				if(std::visit(Engine::Physics::CollisionChecker{},
							  bullet->get_component<Collision>().shape,
							  enemy->get_component<Collision>().shape))
				{
					auto& enemy_hp = enemy->get_component<Hitpoints>();
					if(!enemy_hp.invulnerable)
					{
						/// @todo damage component for bullet
						enemy_hp.current_hp -= 1;
					}

					if(enemy_hp.current_hp <= 0)
					{
						enemy->destroy();
						score_ += enemy->get_component<Score>().score;
					}

					auto pos = bullet->get_component<Transform>().pos;
					for(int i{0}; i < 10; ++i)
					{
						auto rng_dir = get_random_dir();
						particles_.create(pos.x, pos.y, rng_dir.x * i, rng_dir.y * i, 15 + i);
					}

					bullet->destroy();
				}
			}
		}

		// collision player <-> enemy
		for(auto& enemy : entities_.get_entities("enemy"))
		{
			assert(enemy->get_component<Collision>() && "Enemy has no Collision component");

			if(std::visit(Engine::Physics::CollisionChecker{},
						  player_->get_component<Collision>().shape,
						  enemy->get_component<Collision>().shape))
			{
				auto& player_hp = player_->get_component<Hitpoints>();
				if(!player_hp.invulnerable)
				{
					/// @todo damage component for bullet
					player_hp.current_hp -= 1;
				}

				if(player_hp.current_hp <= 0)
				{
					game_over_ = true;
				}

				// destroy the enemy, so the player won't collide the next frame again
				enemy->destroy();
			}
		}
	}

	void SceneAsteroids::update()
	{
		// user_input();
		if(!is_paused_ && !game_over_)
		{
			spawn_entities();
			reduce_lifespan();
			entities_.update();
			particles_.update();
			movement();
			collision();
		}
		current_frame_++;
	}

	void SceneAsteroids::render()
	{
		using namespace Engine::Components;

		auto& window = game_->window();
		auto  view	 = window.getDefaultView();

		// from now on, fill the blank canvas
		window.clear();

		// firstly set up the view on the players spaceship
		const auto& player_transf = player_->get_component<Transform>();
		const auto	view_size	  = view.getSize();
		const auto	view_min_x	  = view_size.x / 2;
		const auto	view_min_y	  = view_size.y / 2;
		const auto	view_max_x	  = world_size_.x - view_min_x;
		const auto	view_max_y	  = world_size_.y - view_min_y;

		auto view_point = sf::Vector2f{std::clamp(player_transf.pos.x, view_min_x, view_max_x),
									   std::clamp(player_transf.pos.y, view_min_y, view_max_y)};
		view.setCenter(view_point);
		window.setView(view);

		// next draw the background (bottom layer)
		window.draw(background_);

		if(game_over_)
		{
			sf::Text game_over;
			game_over.setFont(game_->assets().get_font("Gidole"));
			game_over.setString("GAME OVER");
			game_over.setCharacterSize(48);	 // in pixels
			game_over.setFillColor(sf::Color::White);
			game_over.setStyle(sf::Text::Bold);
			game_over.setPosition(window.mapPixelToCoords(
				{static_cast<int>(view_min_x), static_cast<int>(view_min_y)}, view));
			Utility::center_text(game_over);
			window.draw(game_over);

			sf::Text score;
			score.setFont(game_->assets().get_font("Gidole"));
			score.setString(std::format("Score {:>04}", score_));
			score.setCharacterSize(24);	 // in pixels
			score.setFillColor(sf::Color::White);
			score.setStyle(sf::Text::Bold);
			score.setPosition(window.mapPixelToCoords(
				{static_cast<int>(view_min_x), static_cast<int>(1.3 * view_min_y)}, view));
			Utility::center_text(score);
			window.draw(score);

			// finally display all rendered content
			window.display();
			return;
		}

		// then fill the vastness of space with entities (space ship, lasers, asteroids..)
		for(auto& entity : entities_.get_entities())
		{
			if(auto& hitbox = entity->get_component<Collision>(); hitbox && draw_hitboxes_)
			{
				std::visit(Overload{[&window](sf::RectangleShape& shape) { window.draw(shape); },
									[&window](sf::CircleShape& shape) { window.draw(shape); }},
						   hitbox.shape);
			}

			if(auto& shape = entity->get_component<Drawable>(); shape && draw_sprites_)
			{
				// assume every shape has also a transform component
				assert(entity->get_component<Transform>() && "Missing transform component");
				// shape.update();
				auto  sprite = shape.get_sprite();
				auto& transf = entity->get_component<Transform>();

				sprite.setPosition({transf.pos.x, transf.pos.y});
				sprite.setRotation(sf::degrees(transf.angle + shape.get_rotation()));
				window.draw(sprite);
			}
		}

		particles_.draw(window);

		const auto hp = player_->get_component<Hitpoints>().current_hp;

		// draw HUD elements on top
		sf::Text text;
		text.setFont(game_->assets().get_font("Gidole"));
		text.setString(std::format("Score {:>04}\nHealth {}", score_, hp));
		text.setCharacterSize(24);	// in pixels
		text.setFillColor(sf::Color::White);
		text.setStyle(sf::Text::Bold);
		text.setPosition(window.mapPixelToCoords({10, 10}, view));
		window.draw(text);

		// finally display all rendered content
		window.display();
	}

	void SceneAsteroids::do_action(const Engine::Systems::Action& action)
	{
		using namespace Engine::Components;

		try
		{
			if(action.type() == Engine::Systems::ActionType::Start)
			{
				if(action.name() == "Quit")
				{
					game_->change_scene("MainMenu");
					return;
				}
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
				if(action.name() == "Reset")
				{
					game_over_ ? [&]() { reset(); }() : [&]() { game_over_ = true; }();
				}
				if(action.name() == "ToggleSprites")
				{
					draw_sprites_ = !draw_sprites_;
				}
				if(action.name() == "ToggleHitboxes")
				{
					draw_hitboxes_ = !draw_hitboxes_;
				}
			}
			else if(action.type() == Engine::Systems::ActionType::End)
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
		catch(const std::exception& e)
		{
			std::cerr << std::format("Exception caught {} in {}\n", e.what(), __FILE__);
		}
	}

	void SceneAsteroids::on_end() {}


	void SceneAsteroids::reset()
	{
		for(auto entity : entities_.get_entities())
		{
			entity->destroy();
		}
		game_over_ = false;
		score_	   = 0;
		spawn_player();
	}

}  // namespace Engine::Scene