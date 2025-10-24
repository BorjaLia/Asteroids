#include "AsteroidManager.h"

int currentAsteroid = 0;

float maxOutsideLifetime = 10.0f;
float maxInsideLifetime = 25.0f;

void asteroidManager::Init(AsteroidSpawner& spawner)
{
	spawner.active = true;
}

int asteroidManager::AvailableAsteroid(asteroid::Asteroid asteroids[], int asteroidAmount)
{
	for (int i = 0; i < asteroidAmount; i++)
	{
		if (!asteroids[i].active) {
			return i;
		}
	}
	return 0;
}

void asteroidManager::Update(AsteroidSpawner& spawner, asteroid::Asteroid asteroids[], int asteroidAmount)
{
	Spawner(spawner, asteroids, asteroidAmount);
	Destroyer(spawner, asteroids, asteroidAmount);
}

void asteroidManager::Spawner(AsteroidSpawner& spawner, asteroid::Asteroid asteroids[], int asteroidAmount)
{
	if (!spawner.active) {
		return;
	}

	if (spawner.activeAsteroidSlots >= spawner.activeAsteroidSlotsLimit) {
		return;
	}

	spawner.timeSinceLastSpawn += rend::deltaTime;

	if (spawner.timeSinceLastSpawn <= spawner.spawnInterval) {
		return;
	}
	spawner.timeSinceLastSpawn = 0.0f;

	int availableAsteroidID = AvailableAsteroid(asteroids, asteroidAmount);
	availableAsteroidID = currentAsteroid;

	if (rend::frameInfo) {
		std::cout << '\n';
		std::cout << "spawned asteroid n " << availableAsteroidID << '\n';
	}

	Spawn(spawner, asteroids[availableAsteroidID]);
}

void asteroidManager::Spawn(AsteroidSpawner& spawner, asteroid::Asteroid& asteroid)
{
	if (!spawner.active) {
		return;
	}

	asteroid::Reset(asteroid);

	asteroid::Activate(asteroid);

	asteroid::Type asteroidType;

	asteroidType = static_cast<asteroid::Type>(mth::GetRandom(static_cast<int>(asteroid::Type::SMALL), static_cast<int>(asteroid::Type::SIZE) - 1));

	switch (asteroidType)
	{
	default:
		asteroidType = asteroid::Type::SMALL;
	case asteroid::Type::SMALL: {
		asteroid.size = { 0.015f,0.015f };
		asteroid.mass = 250.0f;

		asteroid.asteroidParticleActivator.lifetime = { 0.25f,0.5f };
		asteroid.asteroidParticleActivator.minSize = { 0.00025f,0.00025f };
		asteroid.asteroidParticleActivator.maxSize = { 0.00125f,0.00125f };
		break;
	}
	case asteroid::Type::MEDIUM: {
		asteroid.size = { 0.03f,0.03f };
		asteroid.mass = 650.0f;

		asteroid.asteroidParticleActivator.lifetime = { 0.5f,0.75f };
		asteroid.asteroidParticleActivator.minSize = { 0.0005f,0.0005f };
		asteroid.asteroidParticleActivator.maxSize = { 0.0025f,0.0025f };
		break;
	}
	case asteroid::Type::BIG: {
		asteroid.size = { 0.05f,0.05f };
		asteroid.mass = 1000.0f;

		asteroid.asteroidParticleActivator.lifetime = { 0.75f,0.95f };
		asteroid.asteroidParticleActivator.minSize = { 0.00075f,0.00075f };
		asteroid.asteroidParticleActivator.maxSize = { 0.005f,0.005f };
		break;
	}
	}

	asteroid.size *= 1.0f + mth::GetRandomf(-spawner.sizeOffsetPercent, spawner.sizeOffsetPercent);

	asteroid.mass *= 1.0f + mth::GetRandomf(-spawner.massOffsetPercent, spawner.massOffsetPercent);

	asteroid.asteroidParticleActivator.startingOffset = asteroid.size;

	asteroid.type = asteroidType;

	vec::Vector2 spawnPos = { 0.0f,1.0f };

	spawnPos.toCircularDegree();

	spawnPos.y = mth::GetRandomf(spawner.spawnAngle.x, spawner.spawnAngle.y);
	spawnPos.x = mth::GetRandomf(spawner.spawnAreaExclusionRadius, spawner.spawnAreaRadius);

	spawnPos.toCartesianDegree();

	asteroid.pos = spawnPos + spawner.center;
	if (rend::frameInfo) {
		std::cout << "Pos : " << spawnPos << '\n';
	}

	vec::Vector2 spawnDirection = spawner.center - asteroid.pos;

	spawnDirection.randomizeAngle(-spawner.directionSpread / 2.0f, spawner.directionSpread / 2.0f);
	spawnDirection.normalize();

	asteroid.direction = spawnDirection;

	asteroid.speed = mth::GetRandomf(spawner.speed.x, spawner.speed.y) / (mth::Normalized(static_cast<float>(asteroidType)));

	currentAsteroid++;
	if (currentAsteroid >= asteroidManager::maxAsteroidAmount) {
		currentAsteroid = 0;
	}

	spawner.activeAsteroidSlots++;

	asteroid.size.x /= rend::windowRatio;

	asteroid.active = true;

	if (rend::frameInfo) {
		std::cout << "At " << asteroid.pos << '\n';
		std::cout << "Going to " << asteroid.direction.asCircularDegree() << '\n';
		std::cout << "speed " << asteroid.speed << '\n';
	}
}

void asteroidManager::Destroyer(AsteroidSpawner& spawner, asteroid::Asteroid asteroids[], int asteroidAmount)
{
	bool destroy = false;
	for (int i = 0; i < asteroidAmount; i++)
	{
		destroy = false;
		if (asteroids[i].inside) {
			destroy = (asteroids[i].lifetime >= maxInsideLifetime) || (asteroids[i].Health <= 0.0f);
		}
		else {
			destroy = (asteroids[i].lifetime >= maxOutsideLifetime);
		}
		if (destroy) {
			asteroid::Reset(asteroids[i]);
			spawner.activeAsteroidSlots--;
			if (rend::frameInfo) {
				std::cout << '\n';
				std::cout << "Destroyed asteroid n " << i << '\n';
			}
		}
	}
}

bool asteroidManager::Collision(ship::Ship& ship, bullet::Bullet bullets[], int bulletAmount, asteroid::Asteroid asteroids[], int asteroidAmount)
{
	for (int a = 0; a < asteroidAmount; a++)
	{
		for (int b = 0; b < bulletAmount; b++)
		{
			if (asteroids[a].pos == bullets[b].pos || asteroids[a].pos == ship.pos) {
				return true;
			}
		}
	}
	return false;
}
