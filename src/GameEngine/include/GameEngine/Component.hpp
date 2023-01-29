/// @file Component.hpp
/// @brief Defines the data holding components of entities
/// @details Components inherit from a base class, that holds a value if the component is present
/// @see Entity Component System
/// @date 2022.11.16
/// @author Hendrik Poettker

#pragma once
#include <GameEngine/Animation.hpp>
#include <GameMath/Vec2.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <format>
#include <iostream>
#include <tuple>
#include <variant>

namespace Engine::Components
{
	/// @brief Base Component interface, determines if a component was added to an entity
	struct BaseComponent
	{
		bool has = false;  //!< controls if a component is active

		/// @brief Eases use of component in if statements by implicitly converting
		explicit operator bool() const noexcept
		{
			return has;
		};
	};

	/// @brief Holds data related to movement, position and angle
	struct Transform : public BaseComponent
	{
		Transform(Vec2 pos = {0.f, 0.f}, Vec2 vel = {0.f, 0.f}, float angle = 0.f)
			: pos(pos), prev_pos(pos), vel(vel), angle(angle)
		{
		}

		Vec2  pos;
		Vec2  prev_pos;
		Vec2  vel;
		Vec2  scale = {1.f, 1.f};
		float angle;
	};


	/// @brief Holds data related to animation and texture
	/// @details This component slightly violates the ECS pattern, by having utility functions
	/// inside a class, that is supposed to hold data only. However the advantage of this deviation
	/// is, that common functionality pertaining to animations does not have to be reimplemented for
	/// each scene.
	struct Drawable
		: public BaseComponent
		, public Animation
	{
		Drawable() = default;

		Drawable(const std::string& name, sf::Texture& texture) : Animation(name, texture) {}


		Drawable(const std::string& name, sf::Texture& texture, size_t frame_count, size_t speed)
			: Animation(name, texture, frame_count, speed)
		{
		}
	};

	struct Sound : public BaseComponent
	{
		Sound() = default;

		sf::Sound sound;
	};

	using CollShape = std::variant<sf::CircleShape, sf::RectangleShape>;

	/// @brief Holds the shape, that is used to check for collisions with another entity.
	struct Collision : public BaseComponent
	{
		explicit Collision(CollShape hitbox = CollShape{}) : shape(std::move(hitbox)){};

		CollShape shape;
	};

	/// @brief Holds an amount of value attributed to this entity.
	struct Score : public BaseComponent
	{
		/// @brief Constructor that sets the score of an entity
		explicit Score(int score = 0) : score(score) {}

		int score;	//!< value of an entity
	};

	/// @brief Shows certain doom aproaching
	struct Lifespan : public BaseComponent
	{
		explicit Lifespan(int total = 0) : remaining(total), total(total) {}

		int remaining;	//!< value between 0 and total
		int total;		//!< initial duration until destruction
	};

	/// @brief This component indicates the entity may use key press input
	struct Input : public BaseComponent
	{
		bool up	   = false;
		bool left  = false;
		bool right = false;
		bool down  = false;
		bool space = false;
	};

	/// @brief This component indicates the entity may use mouse input
	struct Mouse : public BaseComponent
	{
		bool  lmb = false;	//!< left mouse button click
		bool  rmb = false;	//!< right mouse button click
		float x	  = 0.F;	//!< mouse position
		float y	  = 0.F;	//!< mouse position
	};

	/// @brief Holds the amount of damage an entity can withstand
	struct Hitpoints : public BaseComponent
	{
		explicit Hitpoints(int start_hp = 1) : max_hp(start_hp), current_hp(start_hp) {}

		int	 max_hp;
		int	 current_hp;
		bool invulnerable = false;
	};

	using ShaderName	  = std::string;
	using ShaderParamName = std::string;
	using ShaderParamVal  = std::variant<int, float, const sf::Texture*>;
	using ShaderParamMap  = std::unordered_map<ShaderParamName, ShaderParamVal>;
	using ShaderParamMap  = std::unordered_map<ShaderParamName, ShaderParamVal>;

	struct Shader : public BaseComponent
	{
		ShaderName	   name;		///< matches the name of the shader file in the asset manager
		ShaderParamMap parameters;	///< all parameters that need to be passed to the shader

		/// @brief Constructs a shader component, currently an entity may have only one shader
		/// @param shader_name how the shader is referred to in the Assets class
		/// @param parameter_map all parameters that need to be passed to the shader
		Shader(ShaderName	  shader_name	= ShaderName{},
			   ShaderParamMap parameter_map = ShaderParamMap{})
			: name(std::move(shader_name)), parameters(std::move(parameter_map))
		{
		}
	};

	/// @brief Hold all components in one container
	using ComponentTuple = std::tuple<Transform,
									  Drawable,
									  Sound,
									  Collision,
									  Score,
									  Lifespan,
									  Input,
									  Mouse,
									  Hitpoints,
									  Shader>;

}  // namespace Engine::Components