#include "Particles.h"

class ParticlesCreator;

Particles::~Particles()
{
	particles.clear();
}

void Particles::Shoot( int screenWidth, int screenHeight )
{
	//cannon is responsible for shot angle and shot force
	float angle = (float)M_PI / 2.f * Utils::RndFromTo_F(0.75f, 1.25f); //68.5 degrees to 112.5 degrees
	float force = 350.f * Utils::RndFromTo_F( 0.9f, 1.1f );
	std::vector<Particle*> created = Particle::CreateByShooting( (float)(screenWidth / 2.f), (float)screenHeight, force, angle );
	RegisterParticles( created );
}

void Particles::Update( float step )
{
	for (Particle* particle : particles)
	{
		particle->Update( step );
	}

	for (auto part : partsToExplode)
	{
		RegisterParticles( Particle::CreateByExploding( part ) );

		bodies.remove( &part->GetView() );
		particles.remove( part );
		delete part;
	}

	partsToExplode.clear();
}

const std::list<const ParticleBody*>& Particles::GetAllData() const
{
	return bodies;
}

void Particles::RegisterParticles( const std::vector<Particle*>& partsToRegister )
{
	for (Particle* part : partsToRegister)
	{
		particles.push_back( part );
		bodies.push_back( &part->GetView() );
		part->onExplode = [this, part]() {
			partsToExplode.push_back( part );
		};
	}
}

