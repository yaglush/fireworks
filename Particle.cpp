#include "Particle.h"

float Particle::bigParticleTimeToExplode;

Particle::Particle( float x, float y, float vx, float vy, float timeToExplode, Kind kind )
	: timeToExplode( timeToExplode )
	, timeToExplodeInitial( timeToExplode )
	, vx( vx )
	, vy( vy )
{
	body.x = x;
	body.y = y;
	body.kind = kind;
}

void Particle::Update( float step )
{
	//step ~ 0.05

	timeToExplode -= step;
	if (timeToExplode <= 0 && onExplode)
	{
		onExplode();
		return;
	}

	//vx ~ 100px
	body.x += vx * step;

	vy += Utils::gravity * step;
	body.y += vy * step;
}

const ParticleBody& Particle::GetView() const
{
	return body;
}

std::vector<Particle*> Particle::CreateByExploding( Particle* source )
{
	std::vector<Particle*> result;
	switch (source->GetView().kind)
	{
	case Kind::Big:
	{
		const int numSmall = Utils::RndFromTo_I( 1, 3 );
		for (int i = 0; i < numSmall; ++i)
		{
			float vx = source->vx * 2.f * Utils::RndFromTo_F( 0.75f, 1.25f );
			float vy = source->vy * 2.f * Utils::RndFromTo_F( 0.75f, 1.25f );
			float timeToExplode = source->timeToExplodeInitial * Utils::RndFromTo_F( 0.35f, 0.65f );
			Particle* part = new Particle( source->GetView().x, source->GetView().y, vx, vy, timeToExplode, Kind::Small );
			result.push_back( part );
		}

		const int numSparks = Utils::RndFromTo_I( 10, 100 );
		for (int i = 0; i < numSparks; ++i)
		{
			float angle = (float)M_PI * 2.f * Utils::RndFrom0to1();
			float force = 100.f * source->timeToExplodeInitial;
			float vx = cosf( angle ) * force;
			float vy = -sinf( angle ) * force;

			float timeToExplode = source->timeToExplodeInitial * Utils::RndFromTo_F( 0.35f, 0.65f );
			Particle* part = new Particle( source->GetView().x, source->GetView().y, vx, vy, timeToExplode, Kind::Spark );
			result.push_back( part );
		}
	}
	break;

	case Kind::Small:
	{
		const int numSparks = Utils::RndFromTo_I( 10, 100 );
		for (int i = 0; i < numSparks; ++i)
		{
			float angle = (float)M_PI * 2.f * Utils::RndFrom0to1();
			float force = 100.f * Particle::bigParticleTimeToExplode;
			float vx = cosf( angle ) * force;
			float vy = -sinf( angle ) * force;

			float timeToExplode = source->timeToExplodeInitial * Utils::RndFromTo_F( 0.35f, 0.65f );
			Particle* part = new Particle( source->GetView().x, source->GetView().y, vx, vy, timeToExplode, Kind::Spark );
			result.push_back( part );
		}
	}

	default:
		break;

	}

	//Imperfect solution: we return vector as a copy
	return result;
}

std::vector<Particle*> Particle::CreateByShooting( float x, float y, float force, float angle )
{
	std::vector<Particle*> result;

	//particle is responsible for it's fuse (time to explode) and charge (vector)
	float timeToExplode = Utils::RndFromTo_F( 1.f, 5.f );

	float vx = cosf( angle ) * force;
	float vy = -sinf( angle ) * force;
	Particle* part = new Particle( x, y, vx, vy, timeToExplode, Kind::Big );
	result.push_back( part );

	bigParticleTimeToExplode = timeToExplode;

	//Imperfect solution: we return vector as a copy
	return result;
}