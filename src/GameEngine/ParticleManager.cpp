#include <GameEngine/ParticleManager.hpp>

void ParticleManager::draw(sf::RenderTarget& target, const sf::RenderStates& states) const 
{
	// apply the transform
	//states.transform *= getTransform();

	// our particles don't use a texture
	//states.texture = NULL;

	// draw the vertex array
	target.draw(vertices_, states);
}
