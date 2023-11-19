#include <CodeHelpers/Logger.hpp>
#include <CodeHelpers/Overload.hpp>
#include <GameEngine/Collision.hpp>
#include <GameEngine/GameEngine.hpp>
#include <GameEngine/Utility.hpp>
#include <GameScenes/Asteroids.hpp>
#include <fmt/core.h>
#include <random>

class Vec2;

namespace Engine::Scene
{
	/// @brief Anonymous namespace for SceneAsteroids.cpp
	namespace
	{
		struct on_player_death
		{
		};

		struct Name
		{
			std::string name;
		};

		struct Background
		{
		};

		struct Bullet
		{
		};

		struct Asteroid
		{
		};

		enum class Layer
		{
			Background,
			Asteroids,
			Laser,
			Player,
			HUD
		};

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
		void move_player(entt::registry& registry, entt::entity player)
		{
			using namespace Engine::Components;
			auto& transf = registry.get<Transform>(player);

			if(registry.get<Input>(player).up)
			{
				transf.pos += direction_from_degree(transf.angle) * 4.F;
			}
			if(registry.get<Input>(player).down)
			{
				transf.pos -= direction_from_degree(transf.angle) * 4.F;
			}

			transf.angle += registry.get<Input>(player).right ? 2.F : 0.F;
			transf.angle += registry.get<Input>(player).left ? -2.F : 0.F;
		}

	}  // namespace

	using namespace CodeHelper;
	using namespace GameSystems;
	namespace rn = std::ranges;

	Asteroids::Asteroids(GameEngine* engine) : BaseScene(engine)
	{
		dispatcher_.sink<score_changed>().connect<&Score::on_score_changed>(score_);
		dispatcher_.sink<on_player_death>().connect<&Score::reset>(score_);
		init();
	};

	void Asteroids::spawn_entities()
	{
		using namespace Engine::Components;

		auto&		pos = registry_.get<Transform>(player2_).pos;
		static auto bullet_cooldown{0};
		if(registry_.get<Input>(player2_).space && bullet_cooldown == 0)
		{
			auto	   angle  = registry_.get<Transform>(player2_).angle;
			auto	   dir	  = direction_from_degree(angle);
			const auto radius = 4.F;
			auto	   circle = sf::CircleShape(radius);
			circle.setOrigin({radius, radius});

			auto bullet = registry_.create();
			registry_.emplace<Bullet>(bullet);
			registry_.emplace<Layer>(bullet, Layer::Laser);
			registry_.emplace<Transform>(bullet, pos + (dir * 20), dir * 25, angle + 90.F);
			registry_.emplace<Collision>(bullet, circle);
			registry_.emplace<Lifespan>(bullet, 120);
			registry_.emplace<Drawable>(bullet, "Laser", game_->assets().get_texture("Laser"));

			auto& sound = registry_.emplace<Sound>(bullet).sound;
			sound.setBuffer(game_->assets().get_sound("sfx_laser1"));
			sound.play();

			bullet_cooldown = 10;
		}
		bullet_cooldown = std::max(0, --bullet_cooldown);

		static auto enemy_cooldown{0};
		if(enemy_cooldown == 0 && registry_.view<Asteroid>().size() <= 15)
		{
			const auto nr = get_random_int(0, 9);

			Vec2 enemy_pos{0.F, 0.F};
			do
			{
				enemy_pos = get_random_start_pos(static_cast<int>(world_size_.x),
												 static_cast<int>(world_size_.y));
			} while(get_distance_sq(enemy_pos, pos) < 100 * 100);

			auto dir   = get_random_dir();
			auto enemy = registry_.create();
			registry_.emplace<Transform>(enemy, enemy_pos, dir * 2.F);
			switch(nr)
			{
				default:
					[[fallthrough]];
				case 0:
					[[fallthrough]];
				case 1:
					registry_.emplace<Score>(enemy, 5);
					registry_.emplace<Hitpoints>(enemy, 1);
					break;
				case 2:
					[[fallthrough]];
				case 3:
					registry_.emplace<Score>(enemy, 10);
					registry_.emplace<Hitpoints>(enemy, 1);
					break;
				case 4:
					[[fallthrough]];
				case 5:
					registry_.emplace<Score>(enemy, 20);
					registry_.emplace<Hitpoints>(enemy, 2);
					break;
				case 6:
					[[fallthrough]];
				case 7:
					registry_.emplace<Score>(enemy, 20);
					registry_.emplace<Hitpoints>(enemy, 2);
					break;
				case 8:
					[[fallthrough]];
				case 9:
					registry_.emplace<Score>(enemy, 50);
					registry_.emplace<Hitpoints>(enemy, 5);
					break;
			}

			auto& texture = game_->assets().get_texture("Meteor" + std::to_string(nr));
			registry_.emplace<Collision>(enemy, Utility::get_circle_from(texture));
			registry_.emplace<Asteroid>(enemy);
			registry_.emplace<Drawable>(enemy, "Meteor", texture);
			enemy_cooldown = 30;
		}
		enemy_cooldown = std::max(0, --enemy_cooldown);
	}

	void Asteroids::spawn_planet()
	{
		using namespace Engine::Components;
		auto& texture = game_->assets().get_texture("Planet");
		auto  planet  = registry_.create();
		registry_.emplace<Layer>(planet, Layer::Background);
		registry_.emplace<Drawable>(planet, "Planet", texture).set_rotation(get_random_int(0, 360));
		registry_.emplace<Transform>(planet, Vec2{world_size_.x / 2, world_size_.y / 2});
		registry_.emplace<Background>(planet);
	}

	void Asteroids::spawn_player()
	{
		using namespace Engine::Components;
		auto& texture = game_->assets().get_texture("PlayerShip");

		player2_ = registry_.create();
		registry_.emplace<Layer>(player2_, Layer::Player);
		registry_.emplace<Drawable>(player2_, "PlayerShip", texture).set_rotation(90.F);
		registry_.emplace<Collision>(player2_, Utility::get_circle_from(texture));
		registry_.emplace<Transform>(player2_, Vec2{world_size_.x / 2, world_size_.y / 2});
		registry_.emplace<Hitpoints>(player2_, 3);
		registry_.emplace<Input>(player2_);
	}

	void Asteroids::reduce_lifespan()
	{
		using namespace Engine::Components;
		for(auto entity : registry_.view<Lifespan>())
		{
			auto& remaining = registry_.get<Lifespan>(entity).remaining;
			(remaining > 0) ? [&]() { remaining--; }() : [=]() { registry_.destroy(entity); }();
		}
	}

	void Asteroids::movement()
	{
		using namespace Engine::Components;

		move_player(registry_, player2_);

		auto x_min = 0.F;
		auto y_min = 0.F;
		auto x_max = world_size_.x;
		auto y_max = world_size_.y;

		auto transformables = registry_.view<Transform, Collision>();
		for(auto& entity : transformables)
		{
			auto& transf = registry_.get<Transform>(entity);
			auto& hitbox = registry_.get<Collision>(entity);

			transf.prev_pos = transf.pos;
			auto& x			= transf.pos.x;
			auto& dx		= transf.vel.x;
			auto& y			= transf.pos.y;
			auto& dy		= transf.vel.y;
			x				= std::clamp(x + dx, x_min, x_max);
			y				= std::clamp(y + dy, y_min, y_max);

			std::visit(Overload{[x, y](sf::RectangleShape& shape) {
									shape.setPosition({x, y});
								},
								[x, y](sf::CircleShape& shape) {
									shape.setPosition({x, y});
								}},
					   hitbox.shape);
		}

		// destroy bullets that leave the play space
		for(auto& bullet : registry_.view<Bullet>())
		{
			auto bullet_x = registry_.get<Transform>(bullet).pos.x;
			auto bullet_y = registry_.get<Transform>(bullet).pos.y;
			if(bullet_x <= x_min || bullet_x >= x_max || bullet_y <= y_min || bullet_y >= y_max)
			{
				registry_.destroy(bullet);
			}
		}

		// have enemies bounce from the borders of the screen
		for(auto& enemy : registry_.view<Asteroid>())
		{
			auto& transf  = registry_.get<Transform>(enemy);
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

	void Asteroids::init()
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

	void Asteroids::collision()
	{
		bullet_collision();
		player_collision();
	}

	void Asteroids::bullet_collision()
	{
		using namespace Engine::Components;
		for(auto bullet : registry_.view<Bullet>())
		{
			for(auto enemy : registry_.view<Asteroid>())
			{
				if(std::visit(Engine::Physics::CollisionChecker{},
							  registry_.get<Collision>(bullet).shape,
							  registry_.get<Collision>(enemy).shape))
				{
					auto& enemy_hp = registry_.get<Hitpoints>(enemy);
					if(!enemy_hp.invulnerable)
					{
						/// @todo damage component for bullet
						enemy_hp.current_hp -= 1;
					}

					if(enemy_hp.current_hp <= 0)
					{
						dispatcher_.enqueue(score_changed(registry_.get<Score>(enemy).score));
						registry_.destroy(enemy);
					}

					auto pos = registry_.get<Transform>(bullet).pos;
					for(int i{0}; i < 10; ++i)
					{
						auto rng_dir = get_random_dir();
						particles_.create(pos.x, pos.y, rng_dir.x * i, rng_dir.y * i, 15 + i);
					}

					registry_.destroy(bullet);
					break;
				}
			}
		}
	}

	void Asteroids::player_collision()
	{
		using namespace Engine::Components;
		for(auto enemy : registry_.view<Asteroid>())
		{
			if(std::visit(Engine::Physics::CollisionChecker{},
						  registry_.get<Collision>(player2_).shape,
						  registry_.get<Collision>(enemy).shape))
			{
				auto& player_hp = registry_.get<Hitpoints>(player2_);
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
				registry_.destroy(enemy);
				break;
			}
		}
	}

	void Asteroids::update()
	{
		if(!is_paused_ && !game_over_)
		{
			spawn_entities();
			reduce_lifespan();
			movement();
			collision();
			particles_.update();
			dispatcher_.update();
		}
		current_frame_++;
	}

	void Asteroids::render()
	{
		using namespace Engine::Components;

		auto& window = game_->window();
		auto  view	 = window.getDefaultView();

		// from now on, fill the blank canvas
		window.clear();

		// firstly set up the view on the players spaceship
		const auto& player_transf = registry_.get<Transform>(player2_);
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
			score.setString(fmt::format("Score {:>04}", score_.get()));
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

		if(draw_hitboxes_)
		{
			for(auto entity : registry_.view<Collision>())
			{
				std::visit(Overload{[&window](sf::RectangleShape& shape) { window.draw(shape); },
									[&window](sf::CircleShape& shape) { window.draw(shape); }},
						   registry_.get<Collision>(entity).shape);
			}
		}

		if(draw_sprites_)
		{
			auto draw = [&](entt::entity entity)
			{
				const auto transf = registry_.get<Transform>(entity);
				auto&	   shape  = registry_.get<Drawable>(entity);
				auto	   sprite = shape.get_sprite();
				sprite.setPosition({transf.pos.x, transf.pos.y});
				sprite.setRotation(sf::degrees(transf.angle + shape.get_rotation()));
				window.draw(sprite);
			};
			rn::for_each(registry_.view<Background>(), [draw](auto entity) { draw(entity); });
			rn::for_each(registry_.view<Asteroid>(), [draw](auto entity) { draw(entity); });
			rn::for_each(registry_.view<Bullet>(), [draw](auto entity) { draw(entity); });
			draw(player2_);
		}

		particles_.draw(window);

		// draw HUD elements on top
		const auto hp = registry_.get<Hitpoints>(player2_).current_hp;
		sf::Text   text;
		text.setFont(game_->assets().get_font("Gidole"));
		text.setString(fmt::format("Score {:>04}\nHealth {}", score_.get(), hp));
		text.setCharacterSize(24);	// in pixels
		text.setFillColor(sf::Color::White);
		text.setStyle(sf::Text::Bold);
		text.setPosition(window.mapPixelToCoords({10, 10}, view));
		window.draw(text);

		// finally display all rendered content
		window.display();
	}

	void Asteroids::do_action(const Engine::Systems::Action& action)
	{
		using namespace Engine::Components;

#define MAP_STRING_TO(string, code) \
	if(action.name() == string)     \
	{                               \
		code                        \
	}

		try
		{
			auto& input = registry_.get<Input>(player2_);
			if(action.type() == Engine::Systems::ActionType::Start)
			{
				MAP_STRING_TO("Quit", game_->change_scene("MainMenu"); return;)
				MAP_STRING_TO("Up", input.up = true;)
				MAP_STRING_TO("Down", input.down = true;)
				MAP_STRING_TO("Left", input.left = true;)
				MAP_STRING_TO("Right", input.right = true;)
				MAP_STRING_TO("Shoot", input.space = true;)
				MAP_STRING_TO("Pause", set_paused(!is_paused_);)
				MAP_STRING_TO(
					"Reset", game_over_ ? [&]() { reset(); }() : [&]() { game_over_ = true; }();)
				MAP_STRING_TO("ToggleSprites", draw_sprites_ = !draw_sprites_;)
				MAP_STRING_TO("ToggleHitboxes", draw_hitboxes_ = !draw_hitboxes_;)
			}
			else if(action.type() == Engine::Systems::ActionType::End)
			{
				MAP_STRING_TO("Up", input.up = false;)
				MAP_STRING_TO("Down", input.down = false;)
				MAP_STRING_TO("Left", input.left = false;)
				MAP_STRING_TO("Right", input.right = false;)
				MAP_STRING_TO("Shoot", input.space = false;)
			}
		}
		catch(const std::exception& e)
		{
			LOG_ERROR(dl, "{}", e.what());
		}
#undef MAP_STRING_TO
	}

	void Asteroids::on_end() {}

	void Asteroids::reset()
	{
		registry_.clear();
		game_over_ = false;
		spawn_planet();
		spawn_player();
		dispatcher_.enqueue(on_player_death{});
	}

}  // namespace Engine::Scene