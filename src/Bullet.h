#pragma once

#include "BorjaLib.h"

namespace bullet {

	const int particlesAmount = 3;

	struct Bullet {

		bool active = false;

		float maxLifetime = 2.0f;
		float currentLifetime = 0.0f;

		float damage = 1.0f;

		float mass = 10.0f;

		float maxSpeed = 7.5f;
		float minSpeed = 0.5f;

		float startingSpeed = 1.5f;
		float speed = 0.0f;
		float deceleration = 0.025f;

		vec::Vector2 pos = { -1.0f,-1.0f };
		vec::Vector2 direction = { 0.0f,0.01f };

		vec::Vector2 size = { 0.005f,0.005f };

		int textureID = 0;

		prtcl::ParticleData particles[particlesAmount];

		prtcl::ParticleActivator bulletParticleActivator;
	};

	void Init(Bullet& bullet, drw::SpriteData& bulletSprite, prtcl::ParticleData bulletParticles[], drw::AnimationData bulletFireAnim);
	void Init(Bullet bullets[], int bulletAmount, drw::SpriteData& bulletSprite, prtcl::ParticleData bulletParticles[], drw::AnimationData bulletFireAnim);

	void Reset(Bullet& bullet);
	void Reset(Bullet bullets[], int bulletAmount);
	
	void Activate(Bullet& bullet);

	void Update(Bullet& bullet);
	void Update(Bullet bullets[], int bulletAmount);

	void Draw(Bullet bullet);
	void Draw(Bullet bullets[], int bulletAmount);
}
