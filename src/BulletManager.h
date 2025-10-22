#pragma once

#include "Bullet.h"
#include "Ship.h"

namespace bulletManager {

	const int maxBulletAmount = 50;

	int AvailableBullet(bullet::Bullet bullets[],int bulletAmount);

	void Shoot(ship::Ship& ship,bullet::Bullet bullets[],int bulletAmount);

}