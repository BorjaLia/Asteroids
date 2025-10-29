#include "AsteroidManager.h"

int currentAsteroid = 0;

float maxOutsideLifetime = 10.0f;
float minLifetime = 0.25f;

void asteroidManager::Init(AsteroidSpawner& spawner)
{
	spawner.active = true;
}

void asteroidManager::Reset(AsteroidSpawner& spawner)
{
	spawner = AsteroidSpawner();
	Init(spawner);
	currentAsteroid = 0;
}

int asteroidManager::AvailableAsteroid(asteroid::Asteroid asteroids[], int asteroidAmount)
{
	for (int i = 0; i < asteroidAmount; i++)
	{
		if (!asteroids[i].active && !asteroids[i].breaking) {
			return i;
		}
	}
	return 0;
}

void asteroidManager::Update(AsteroidSpawner& spawner, ship::Ship& ship, bullet::Bullet bullets[], int bulletAmount, asteroid::Asteroid asteroids[], int asteroidAmount)
{
	Spawner(spawner, asteroids, asteroidAmount);
	int id = -1;
	int asteroidID = Collision(ship, bullets, bulletAmount, asteroids, asteroidAmount, id);
	if (asteroidID > -1) {
		if (id == -1) {
			ship.health -= 1.0f;
			ship.speed += (ship.pos - asteroids[asteroidID].collision).normalized() * (asteroids[asteroidID].mass / ship.mass) * 0.05f;
			asteroids[asteroidID].health -= ship.crashDamage;
			asteroids[asteroidID].direction += (asteroids[asteroidID].collision - ship.pos).normalized() * (asteroids[asteroidID].mass / ship.mass) * 0.05f;
			std::cout << '\n';
			std::cout << "Crashed asteroid n " << asteroidID << ". health" << asteroids[asteroidID].health << '\n';
		}
		else {
			asteroids[asteroidID].health -= bullets[id].damage;
			asteroids[asteroidID].direction += bullets[id].direction * (asteroids[asteroidID].mass / bullets[id].mass) * 0.05f;
			bullet::Reset(bullets[id]);
			std::cout << '\n';
			std::cout << "Shot asteroid n " << asteroidID << ". health" << asteroids[asteroidID].health << '\n';
		}
		if (asteroids[asteroidID].health <= 0.0f) {
			std::cout << '\n';
			std::cout << "Kill!" << '\n';
			ship.score += asteroids[asteroidID].score;
			ship.money += asteroids[asteroidID].money;
		}
	}
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

	if (rend::devInfo != rend::InfoMode::NONE) {
		std::cout << '\n';
		std::cout << "spawned asteroid n " << availableAsteroidID << '\n';
	}

	Spawn(spawner, asteroids[availableAsteroidID],availableAsteroidID);
}

void asteroidManager::Spawn(AsteroidSpawner& spawner, asteroid::Asteroid& asteroid, int id, asteroid::Asteroid parent, bool child)
{
	if (rend::devInfo != rend::InfoMode::NONE) {
		std::cout << "Spawn attempt" << '\n';
	}

	if (!spawner.active) {
		return;
	}

	asteroid::Reset(asteroid);

	asteroid::Activate(asteroid);

	asteroid.id = id;

	asteroid::Type asteroidType;

	if (child) {
		asteroidType = static_cast<asteroid::Type>((static_cast<int>(parent.type)) - 1);
	}
	else {
		asteroidType = static_cast<asteroid::Type>(mth::GetRandom(static_cast<int>(asteroid::Type::SMALL), static_cast<int>(asteroid::Type::SIZE) - 1));
	}

	switch (asteroidType)
	{
	default:
		asteroidType = asteroid::Type::SMALL;
	case asteroid::Type::SMALL: {
		asteroid.health = 1.0f;
		asteroid.size = { 0.015f,0.015f };
		asteroid.mass = 250.0f;

		asteroid.score = 10;
		asteroid.money = 15;

		asteroid.asteroidParticleActivator.lifetime = { 0.25f,0.5f };
		asteroid.asteroidParticleActivator.minSize = { 0.00025f,0.00025f };
		asteroid.asteroidParticleActivator.maxSize = { 0.00125f,0.00125f };
		break;
	}
	case asteroid::Type::MEDIUM: {
		asteroid.health = 2.0f;
		asteroid.size = { 0.03f,0.03f };
		asteroid.mass = 650.0f;

		asteroid.score = 50;
		asteroid.money = 25;

		asteroid.asteroidParticleActivator.lifetime = { 0.5f,0.75f };
		asteroid.asteroidParticleActivator.minSize = { 0.0005f,0.0005f };
		asteroid.asteroidParticleActivator.maxSize = { 0.0025f,0.0025f };
		break;
	}
	case asteroid::Type::BIG: {
		asteroid.health = 3.0f;
		asteroid.size = { 0.05f,0.05f };
		asteroid.mass = 1000.0f;

		asteroid.score = 125;
		asteroid.money = 50;

		asteroid.asteroidParticleActivator.lifetime = { 0.75f,0.95f };
		asteroid.asteroidParticleActivator.minSize = { 0.00075f,0.00075f };
		asteroid.asteroidParticleActivator.maxSize = { 0.005f,0.005f };
		break;
	}
	}

	if (child) {
		asteroid.size = parent.size * mth::GetRandomf(0.4f,0.6f);
		asteroid.mass = parent.mass * mth::GetRandomf(0.4f,0.6f);
	}
	else {
		asteroid.size *= 1.0f + mth::GetRandomf(-spawner.sizeOffsetPercent, spawner.sizeOffsetPercent);
		asteroid.mass *= 1.0f + mth::GetRandomf(-spawner.massOffsetPercent, spawner.massOffsetPercent);
	}

	asteroid.asteroidParticleActivator.startingOffset = asteroid.size;

	asteroid.type = asteroidType;

	vec::Vector2 spawnPos = { 0.0f,1.0f };

	if (child) {
		spawnPos = parent.pos;
	}
	else {
		spawnPos.toCircularDegree();

		spawnPos.y = mth::GetRandomf(spawner.spawnAngle.x, spawner.spawnAngle.y);
		spawnPos.x = mth::GetRandomf(spawner.spawnAreaExclusionRadius, spawner.spawnAreaRadius);

		spawnPos.toCartesianDegree();

		spawnPos += spawner.center;
	}
	asteroid.pos = spawnPos;

	vec::Vector2 spawnDirection = spawner.center - asteroid.pos;

	spawnDirection.randomizeAngle(-spawner.directionSpread / 2.0f, spawner.directionSpread / 2.0f);
	spawnDirection.normalize();

	asteroid.direction = spawnDirection;

	if (child) {
		float mag = asteroid.direction.magnitude();
		asteroid.direction += parent.direction;
		asteroid.direction.normalize();
		asteroid.direction * mag;
	}

	if (child) {
		asteroid.speed = parent.speed * mth::GetRandomf(0.9f, 1.1f);
	}
	else {
		asteroid.speed = mth::GetRandomf(spawner.speed.x, spawner.speed.y) / (mth::Normalized(static_cast<float>(asteroidType)));
	}

	currentAsteroid++;
	if (currentAsteroid >= asteroidManager::maxAsteroidAmount) {
		currentAsteroid = 0;
	}

	spawner.activeAsteroidSlots++;

	if (!child) {
		asteroid.size.x /= rend::windowRatio;
	}
	asteroid.active = true;

	if (rend::devInfo != rend::InfoMode::NONE) {
		if (child) {
			std::cout << "Child ";
		}
		else {
			std::cout << "New ";
		}
		std::cout << "at " << asteroid.pos << '\n';
		std::cout << "Going to " << asteroid.direction.asCircularDegree() << '\n';
		std::cout << "speed " << asteroid.speed << '\n';
		if (rend::devInfo == rend::InfoMode::ADVANCED) {
			std::cout << '\n';
			std::cout << '\n';
			std::cout << "Advanced:" << '\n';
			std::cout << "------------" << '\n';
			std::cout << "Parent:" << '\n';
			std::cout << "id " << parent.id << '\n';
			std::cout << "type " << static_cast<int>(parent.type) << '\n';
			std::cout << "size " << parent.size << '\n';
			std::cout << "lifetime " << parent.lifetime << '\n';
			std::cout << "Child:" << '\n';
			std::cout << "id " << asteroid.id << '\n';
			std::cout << "type " << static_cast<int>(asteroid.type) << '\n';
			std::cout << "size " << asteroid.size << '\n';
			std::cout << "lifetime " << asteroid.lifetime << '\n';
			std::cout << "------------" << '\n';
			std::cout << '\n';
		}
	}
}

void asteroidManager::Destroyer(AsteroidSpawner& spawner, asteroid::Asteroid asteroids[], int asteroidAmount)
{
	bool destroy = false;
	for (int i = 0; i < asteroidAmount; i++)
	{
		destroy = false;
		if (asteroids[i].inside) {
			if (asteroids[i].health <= 0.0f && !asteroids[i].breaking) {
				if (rend::devInfo != rend::InfoMode::NONE) {
					std::cout << '\n';
					std::cout << "Destroyed asteroid n " << i << '\n';
				}
				if (asteroids[i].type != asteroid::Type::SMALL) {
					int id1 = AvailableAsteroid(asteroids, asteroidAmount);
					Spawn(spawner, asteroids[id1],id1, asteroids[i], true);
					int id2 = AvailableAsteroid(asteroids, asteroidAmount);
					Spawn(spawner, asteroids[id2],id2, asteroids[i], true);
					asteroids[id1].direction.toCircular();
					asteroids[id1].direction.y = asteroids[id2].direction.rotatedDegree(180.0f).asCircular().y;
					asteroids[id1].direction.toCartesian();
				}
				asteroid::Break(asteroids[i]);
				spawner.activeAsteroidSlots--;
			}
		}
		else if (asteroids[i].lifetime >= maxOutsideLifetime) {
			asteroid::Reset(asteroids[i]);
		}
	}
}

int asteroidManager::Collision(ship::Ship& ship, bullet::Bullet bullets[], int bulletAmount, asteroid::Asteroid asteroids[], int asteroidAmount, int& id)
{
	for (int a = 0; a < asteroidAmount; a++)
	{
		if (asteroids[a].active && asteroids[a].lifetime >= minLifetime) {

			if (coll::CircleOnCircle(ship.pos, asteroids[a].pos, ship.size.x,asteroids[a].size.x,asteroids[a].collision)) {
				id = -1;
				return a;
			}
			for (int b = 0; b < bulletAmount; b++)
			{
				if (bullets[b].active) {

					if (coll::PointOnCircle(bullets[b].pos, asteroids[a].pos, asteroids[a].size.x)) {
						id = b;
						return a;
					}
				}
			}
		}
	}
	return -1;
}
