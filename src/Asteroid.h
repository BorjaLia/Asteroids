#pragma once

#include "BorjaLib.h"

namespace asteroid {

	enum class Type {

		SMALL = 1,
		MEDIUM,
		BIG,
		SIZE
	};

	const int particlesAmount = 3;

	struct Asteroid {

		bool active = false;

		bool breaking = false;

		bool inside = false;

		float lifetime = 0.0f;

		Type type = Type::SMALL;

		float Health = 5.0f;

		float damage = 1.0f;

		int score = 0;
		int money = 0;

		float maxSpeed = 1.5f;
		float minSpeed = 0.05f;

		float startingSpeed = 0.5f;
		float speed = 0.0f;
		float mass = 1.0f;

		vec::Vector2 pos = { -1.0f,-1.0f };
		vec::Vector2 direction = { 0.0f,0.01f };

		vec::Vector2 size = { 0.05f,0.05f };

		int textureID = 0;

		prtcl::ParticleData particles[particlesAmount];

		prtcl::ParticleActivator asteroidParticleActivator;
	};


	void Init(Asteroid& asteroid, drw::SpriteData& asteroidSprite, prtcl::ParticleData asteroidParticles[], drw::AnimationData asteroidExplosionAnim);
	void Init(Asteroid asteroids[], int asteroidAmount, drw::SpriteData& asteroidSprite, prtcl::ParticleData asteroidParticles[], drw::AnimationData asteroidExplosionAnim);

	void Reset(Asteroid& asteroid);
	void Reset(Asteroid asteroids[], int asteroidAmount);

	void Activate(Asteroid& asteroid);

	void Break(Asteroid& asteroid);

	void Update(Asteroid& asteroid);
	void Update(Asteroid asteroids[], int asteroidAmount);

	void Draw(Asteroid asteroid);
	void Draw(Asteroid asteroids[], int asteroidAmount);
}