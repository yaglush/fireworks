#pragma once

#include <functional>
#include <vector>
#include <memory>

#include "Utils.h"

enum class Kind
{
	Big,
	Small,
	Spark
};

struct ParticleBody
{
	float x;
	float y;
	Kind kind;
};

class Particle final
{
public:
	~Particle() {}

	void Update( float step );

	std::function<void()> onExplode;

	const ParticleBody& GetView() const;

	static std::vector<Particle*> CreateByExploding( Particle* source );
	static std::vector<Particle*> CreateByShooting( float x, float y, float force, float angle );


private:
	Particle() = delete;
	Particle( const Particle& other ) = delete;
	Particle& operator=( const Particle& other ) = delete;
	Particle( float x, float y, float vx, float vy, float timeToExplode, Kind kind );

	float vx;
	float vy;

	float timeToExplode;
	float timeToExplodeInitial;

	// we need this in order to refer to Big projectile's lifetime from Spark projectiles of 2nd generation
	// imperfect solution
	static float bigParticleTimeToExplode;

	ParticleBody body;
};