#pragma once

#include "Asteroid.h"
#include "Bullet.h"
#include "Ship.h"

namespace asteroidManager {

	const int maxAsteroidAmount = 50;

	struct AsteroidSpawner {

		bool active = false;

		int activeAsteroidSlots = 0;
		int activeAsteroidSlotsLimit = 25;

		float spawnInterval = 0.125f;
		float timeSinceLastSpawn = spawnInterval;

		vec::Vector2 center = {0.5f,0.5f};

		float spawnAreaRadius = 0.75f;
		float spawnAreaExclusionRadius = 0.75f;

		vec::Vector2 spawnAngle = {0.0f,360.0f};
		
		float massOffsetPercent = 0.2f;
		float sizeOffsetPercent = 0.2f;

		vec::Vector2 speed = { 0.1f,0.25f };

		float directionSpread = 80.0f;
	};

	void Init(AsteroidSpawner& spawner);

	int AvailableAsteroid(asteroid::Asteroid asteroids[], int asteroidAmount);

	void Update(AsteroidSpawner& spawner, asteroid::Asteroid asteroids[], int asteroidAmount);

	void Spawner(AsteroidSpawner& spawner, asteroid::Asteroid asteroids[], int asteroidAmount);
	void Spawn(AsteroidSpawner& spawner, asteroid::Asteroid& asteroid);
	
	void Destroyer(AsteroidSpawner& spawner, asteroid::Asteroid asteroids[], int asteroidAmount);

	bool Collision(ship::Ship& ship, bullet::Bullet bullets[], int bulletAmount, asteroid::Asteroid asteroids[], int asteroidAmount);
}