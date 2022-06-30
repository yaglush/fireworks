#pragma once

#include <list>
#include <vector>

#include "Particle.h"

class Particles
{
public:
	~Particles();

	void Shoot( int screenWidth, int screenHeight );
	void Update( float step );

	const std::list<const ParticleBody*>& GetAllData() const;

private:
	void RegisterParticles( const std::vector<Particle*>& partsToRegister );
	std::list<const ParticleBody*> bodies;
	std::list<Particle*> particles;

	std::vector<Particle*> partsToExplode;
};