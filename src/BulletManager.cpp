#include "BulletManager.h"

int currentShot = 0;

int bulletManager::AvailableBullet(bullet::Bullet bullets[], int bulletAmount)
{
	for (int i = 0; i < bulletAmount; i++)
	{
		if (!bullets[i].active) {
			return i;
		}
	}
	return 0;
}

void bulletManager::Shoot(ship::Ship& ship, bullet::Bullet bullets[],int bulletAmount)
{
	int availableBulletID = AvailableBullet(bullets,bulletAmount);
	availableBulletID = currentShot;

	bullet::Activate(bullets[availableBulletID]);

	bullets[availableBulletID].pos = ship.pos;
	
	std::cout << mth::RadianToDegree(ship.lookingAt.angle(ship.direction)) << "\n";

	float currentAngle = mth::RadianToDegree(ship.lookingAt.angle(ship.direction));

	if (currentAngle > ship.maxShotAngle && currentAngle < 360.0f - ship.maxShotAngle) {
		std::cout << "Yes" << "\n";
		if (currentAngle > 180.0f) {
			bullets[availableBulletID].direction = ship.direction.rotatedDegree(-ship.maxShotAngle);
		}
		else {
			bullets[availableBulletID].direction = ship.direction.rotatedDegree(ship.maxShotAngle);
		}
	}
	else {
		bullets[availableBulletID].direction = ship.lookingAt;
	}

	currentShot++;
	if (currentShot >= bulletManager::maxBulletAmount) {
		currentShot = 0;
	}
}
