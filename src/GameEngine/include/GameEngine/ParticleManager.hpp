/// @file ParticleManager.hpp
/// @brief ...
/// @details ...
/// @see Initial idea from https://www.sfml-dev.org/tutorials/2.5/graphics-vertex-array.php
/// @date 2022.12.05
/// @author Hendrik Poettker

#pragma once
#include <SFML/Graphics.hpp>

class ParticleManager
	: public sf::Drawable
	, public sf::Transformable
{
  public:
	ParticleManager(unsigned int count = 100)
		: particles_(count)
		, vertices_(sf::Points, count)
		, lifetime_(sf::seconds(3.f))
		, emitter_(0.f, 0.f)
	{
	}

	void set_emitter(sf::Vector2f position)
	{
		emitter_ = position;
	}

	void update(sf::Time elapsed)
	{

		for(std::size_t i = 0; i < particles_.size(); ++i)
		{
			// update the particle lifetime
			Particle& p = particles_[i];
			p.lifetime -= elapsed;

			// if the particle is dead, respawn it
			if(p.lifetime <= sf::Time::Zero)
				reset_particle(i);

			// update the position of the corresponding vertex
			vertices_[i].position += p.velocity * elapsed.asSeconds();

			// update the alpha (transparency) of the particle according to its lifetime
			float ratio			 = p.lifetime.asSeconds() / lifetime_.asSeconds();
			vertices_[i].color.a = static_cast<uint8_t>(ratio * 255);
		}
	}

  protected:
	// Inherited via Drawable
	virtual void draw(sf::RenderTarget& target, const sf::RenderStates& states) const override;

	// virtual void draw(RenderTarget& target, const RenderStates& states) const = 0;
	// virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const
	//{
	//	// apply the transform
	//	states.transform *= getTransform();

	//	// our particles don't use a texture
	//	states.texture = NULL;

	//	// draw the vertex array
	//	target.draw(vertices_, states);
	//}

  private:
	/// @brief POD struct holding lifetime and velocity of a single particle
	struct Particle
	{
		sf::Vector2f velocity;
		sf::Time	 lifetime;
	};


	void reset_particle(std::size_t index)
	{
		// give a random velocity and lifetime to the particle
		float angle				   = (std::rand() % 360) * 3.14f / 180.f;
		float speed				   = (std::rand() % 50) + 50.f;
		particles_[index].velocity = sf::Vector2f(std::cos(angle) * speed, std::sin(angle) * speed);
		particles_[index].lifetime = sf::milliseconds((std::rand() % 2000) + 1000);

		// reset the position of the corresponding vertex
		vertices_[index].position = emitter_;
	}

	std::vector<Particle> particles_;
	sf::VertexArray		  vertices_;
	sf::Time			  lifetime_;
	sf::Vector2f		  emitter_;
};