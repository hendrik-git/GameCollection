/// @file ParticleManager.hpp
/// @brief ...
/// @details ...
/// @see Initial idea from https://www.sfml-dev.org/tutorials/2.5/graphics-vertex-array.php
/// @date 2022.12.05
/// @author Hendrik Poettker

#pragma once
#include <SFML/Graphics.hpp>
#include <variant>
#include <vector>


struct ExplosiveEmitter
{
	int	  framesLeft_ = 0;
	float x_		  = 0.0;
	float y_		  = 0.0;
};

using ParticleEmitter = std::variant<ExplosiveEmitter>;

class Particle : public sf::Drawable
{
  public:
	Particle(){};
	~Particle(){};

	void init(float x, float y, float xVel, float yVel, int lifetime)
	{
		x_			= x;
		y_			= y;
		dx_			= xVel;
		dy_			= yVel;
		framesLeft_ = lifetime;
	}

	void update()
	{
		if(!in_use())
		{
			return;
		}

		framesLeft_--;
		x_ += dx_;
		y_ += dy_;
	}

	bool in_use() const
	{
		return framesLeft_ > 0;
	}

	virtual void draw(sf::RenderTarget&		  target,
					  const sf::RenderStates& states = sf::RenderStates::Default) const override
	{
		if(in_use())
		{
			sf::Vertex v;
			v.position = sf::Vector2f(x_, y_);
			v.color	   = sf::Color::Red;
			target.draw(&v, 1, sf::Points, states);
		}
	}

  private:
	int	  framesLeft_ = 0;
	float x_		  = 0.0;
	float y_		  = 0.0;
	float dx_		  = 0.0;
	float dy_		  = 0.0;
};


/// @brief Is responsible for creating, storing, updating and drawing each particle
class ParticleManager
{
  public:
	ParticleManager(int size = 1'000)
	{
		particles_.resize(size);
	};

	void create(float x, float y, float dx, float dy, int lifetime)
	{
		// Find an available particle.
		for(auto& particle : particles_)
		{
			if(!particle.in_use())
			{
				particle.init(x, y, dx, dy, lifetime);
				return;
			}
		}
	}

	void update()
	{
		for(auto& particle : particles_)
		{
			particle.update();
		}
	}

	void draw(sf::RenderTarget& target, const sf::RenderStates& states = sf::RenderStates::Default)
	{
		for(auto& particle : particles_)
		{
			particle.draw(target, states);
		}
	}

	void resize(size_t max_particle_amount)
	{
		particles_.resize(max_particle_amount);
	}

  private:
	std::vector<Particle>		 particles_;
	std::vector<ParticleEmitter> emitters_;
};