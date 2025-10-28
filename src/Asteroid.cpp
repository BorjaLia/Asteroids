#include "Asteroid.h"

void asteroid::Init(Asteroid& asteroid, drw::SpriteData& asteroidSprite, prtcl::ParticleData asteroidParticles[], drw::AnimationData asteroidExplosionAnim)
{
	asteroid.textureID = asteroidSprite.id;

	asteroid.asteroidParticleActivator.animation = asteroidExplosionAnim;

	

	asteroid::Reset(asteroid);

	prtcl::Init(asteroid.asteroidParticleActivator, asteroidParticles);

	for (int i = 0; i < particlesAmount; i++)
	{
		asteroid.particles[i] = asteroidParticles[i];
	}

	asteroid.size.x /= rend::windowRatio;
	asteroid.pos.x /= rend::windowRatio;
	asteroid.direction.x /= rend::windowRatio;
}

void asteroid::Init(Asteroid asteroids[], int asteroidAmount, drw::SpriteData& asteroidSprite, prtcl::ParticleData asteroidParticles[], drw::AnimationData asteroidExplosionAnim)
{
	for (int i = 0; i < asteroidAmount; i++)
	{
		asteroids[i] = asteroid::Asteroid();
		asteroid::Init(asteroids[i], asteroidSprite, asteroidParticles, asteroidExplosionAnim);
	}
}

void asteroid::Reset(Asteroid& asteroid)
{
	asteroid.asteroidParticleActivator.loop = true;
	asteroid.asteroidParticleActivator.id = 0;
	asteroid.asteroidParticleActivator.animated = true;
	asteroid.asteroidParticleActivator.startingOffset = asteroid.size;
	asteroid.asteroidParticleActivator.startingPosInfluence = 1.0f;
	asteroid.asteroidParticleActivator.amount = asteroid::particlesAmount;
	asteroid.asteroidParticleActivator.lifetime = { 0.5f,0.75f };
	asteroid.asteroidParticleActivator.direction = { 0.1f,1.0f };
	asteroid.asteroidParticleActivator.spread = 45.0f;
	asteroid.asteroidParticleActivator.minSize = { 0.0005f,0.0005f };
	asteroid.asteroidParticleActivator.maxSize = { 0.0025f,0.0025f };
	asteroid.asteroidParticleActivator.pos = asteroid.pos;
	asteroid.asteroidParticleActivator.delay = { 0.0f,0.125f };
	asteroid.asteroidParticleActivator.speed = { 0.005f,0.025f };

	asteroid.lifetime = 0.0f;
	asteroid.pos = asteroid::Asteroid().pos;

	for (int i = 0; i < particlesAmount; i++)
	{
		asteroid.particles[i].pos = asteroid.pos;
	}

	asteroid.inside = false;
	asteroid.breaking = false;
	asteroid.active = false;
}

void asteroid::Reset(Asteroid asteroids[], int asteroidAmount)
{
	for (int i = 0; i < asteroidAmount; i++)
	{
		asteroid::Reset(asteroids[i]);
	}
}

void asteroid::Activate(Asteroid& asteroid)
{
	asteroid.active = true;
}

void asteroid::Break(Asteroid& asteroid)
{
	asteroid.breaking = true;

	asteroid.asteroidParticleActivator.loop = false;
	asteroid.asteroidParticleActivator.startingPosInfluence = 1.0f;
	asteroid.asteroidParticleActivator.startingOffset = asteroid.size * 0.5f;

	asteroid.asteroidParticleActivator.lifetime = { 0.5f,2.0f };
	asteroid.asteroidParticleActivator.direction = { 0.1f,1.0f };
	asteroid.asteroidParticleActivator.spread = 360.0f;
	asteroid.asteroidParticleActivator.minSize = { 0.005f,0.005f };
	asteroid.asteroidParticleActivator.maxSize = { 0.015f,0.015f };
	asteroid.asteroidParticleActivator.pos = asteroid.pos;
	asteroid.asteroidParticleActivator.delay = { 0.0f,0.0f };
	asteroid.asteroidParticleActivator.speed = { 0.05f,0.15f };

	asteroid.pos = asteroid::Asteroid().pos;

	asteroid.lifetime = 0.0f;
	prtcl::Init(asteroid.asteroidParticleActivator, asteroid.particles);

	if (rend::devInfo != rend::InfoMode::NONE) {
		
		std::cout << '\n';
		std::cout << "broke!" << '\n';
	}
}

void asteroid::Update(Asteroid& asteroid)
{
	if (asteroid.breaking) {

		prtcl::Update(asteroid.asteroidParticleActivator, asteroid.particles);
		
		for (int i = 0; i < asteroid::particlesAmount; i++)
		{
			asteroid.particles[i].speed *= 0.99f;
		}

		asteroid.lifetime += rend::deltaTime;
		
		if (asteroid.lifetime >= 3.0f) {
			Reset(asteroid);
			std::cout << '\n';
			std::cout << "stopped break!" << '\n';
		}
	}

	if (!asteroid.active) {
		return;
	}

	asteroid.lifetime += rend::deltaTime;

	asteroid.direction.normalize();
	asteroid.direction = asteroid.direction * asteroid.speed * rend::deltaTime;
	asteroid.pos += asteroid.direction;

	if (asteroid.inside) {
		if (asteroid.pos.x > 1.0f) {
			asteroid.pos.x = 0.0f;
		}
		if (asteroid.pos.x < 0.0f) {
			asteroid.pos.x = 1.0f;
		}

		if (asteroid.pos.y > 1.0f) {
			asteroid.pos.y = 0.0f;
		}
		if (asteroid.pos.y < 0.0f) {
			asteroid.pos.y = 1.0f;
		}

	}
	else {
		bool insideX = asteroid.pos.x - asteroid.size.x > 0.0f && asteroid.pos.x + asteroid.size.x < 1.0f;
		bool insideY = asteroid.pos.y - asteroid.size.y > 0.0f && asteroid.pos.y + asteroid.size.y < 1.0f;
		asteroid.inside = insideX && insideY;

		if (asteroid.inside) {
			asteroid.lifetime = 0.0f;
		}
	}

	asteroid.asteroidParticleActivator.pos = asteroid.pos;
	asteroid.asteroidParticleActivator.direction = asteroid.direction;

	for (int i = 0; i < asteroid::particlesAmount; i++)
	{
		asteroid.particles[i].pos += (asteroid.direction) * 0.99f;
	}

	prtcl::Update(asteroid.asteroidParticleActivator, asteroid.particles);

	if (asteroid.Health <= 0.0f) {
		asteroid::Reset(asteroid);
	}
}

void asteroid::Update(Asteroid asteroids[], int asteroidAmount)
{
	for (int i = 0; i < asteroidAmount; i++)
	{
		asteroid::Update(asteroids[i]);
	}
}

void asteroid::Draw(Asteroid asteroid)
{
	drw::Sprite(drw::spriteDataList[asteroid.textureID], asteroid.pos, asteroid.size);
	for (int i = 0; i < 3; i++)
	{
		prtcl::Draw(asteroid.particles);
	}

	if (!asteroid.inside) {
		return;
	}

	if (asteroid.pos.x + asteroid.size.x > 1.0f) {
		drw::Sprite(drw::spriteDataList[asteroid.textureID], { asteroid.pos.x - 1.0f,asteroid.pos.y }, asteroid.size);
	}
	if (asteroid.pos.x - asteroid.size.x < 0.0f) {
		drw::Sprite(drw::spriteDataList[asteroid.textureID], { asteroid.pos.x + 1.0f,asteroid.pos.y }, asteroid.size);
	}

	if (asteroid.pos.y + asteroid.size.y > 1.0f) {
		drw::Sprite(drw::spriteDataList[asteroid.textureID], { asteroid.pos.x,asteroid.pos.y - 1.0f }, asteroid.size);
	}
	if (asteroid.pos.y - asteroid.size.y < 0.0f) {
		drw::Sprite(drw::spriteDataList[asteroid.textureID], { asteroid.pos.x ,asteroid.pos.y + 1.0f }, asteroid.size);
	}
	if (rend::devInfo != rend::InfoMode::NONE) {
		drw::Circle(asteroid.pos,asteroid.size,RED_B);
	}
}

void asteroid::Draw(Asteroid asteroids[], int asteroidAmount)
{
	for (int i = 0; i < asteroidAmount; i++)
	{
		asteroid::Draw(asteroids[i]);
	}
}
