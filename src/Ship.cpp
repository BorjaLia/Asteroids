#include "Ship.h"

void ship::Init(ship::Ship& ship, drw::SpriteData& shipSprite, prtcl::ParticleData fireParticles[], drw::AnimationData fireAnim)
{
	ship.textureID = shipSprite.id;

	ship.fireParticleActivator.animation = fireAnim;

	ship::Reset(ship, fireParticles);

	ship.active = true;
}

void ship::Reset(ship::Ship& ship, prtcl::ParticleData fireParticles[])
{
	ship.fireParticleActivator.loop = true;
	ship.fireParticleActivator.id = 0;
	ship.fireParticleActivator.animated = true;
	ship.fireParticleActivator.loop = true;
	ship.fireParticleActivator.amount = 15;
	ship.fireParticleActivator.lifetime = ship.fireParticleNormalLifetime;
	ship.fireParticleActivator.direction = { 0.0f,1.0f };
	ship.fireParticleActivator.spread = ship.fireParticleNormalSpread;
	ship.fireParticleActivator.minSize = { 0.0005f,0.0005f };
	ship.fireParticleActivator.maxSize = { 0.004f,0.004f };
	ship.fireParticleActivator.pos = ship.pos;
	ship.fireParticleActivator.delay = { 0.0f,0.25f };
	ship.fireParticleActivator.speed = ship.fireParticleNormalSpeed;

	prtcl::Init(ship.fireParticleActivator, fireParticles);
}

void ship::Input(Ship& ship)
{
	if (ctrl::IsKeyPressed(ship.movementTypeChangeKey)) {
		if (ship.movementType == ship::MovementType::MOUSE) {
			ship.movementType = ship::MovementType::KEYBOARD;
		}
		else {
			ship.movementType = ship::MovementType::MOUSE;
		}
	}

	if (ctrl::IsMouseDown(ship.accelerateButton) || ctrl::IsKeyHeldDown(ship.accelerateKeyboard)) {
		ship.accelerating = true;
	}
	else {
		ship.accelerating = false;
	}

	if (ctrl::IsKeyHeldDown(ship.breakKey)) {
		ship.braking = true;
	}
	else {
		ship.braking = false;
	}

	if (ctrl::IsKeyHeldDown(ship.rotateLeftKeyboard)) {
		ship.rotatingLeft = true;
	}
	else {
		ship.rotatingLeft = false;
	}

	if (ctrl::IsKeyHeldDown(ship.rotateRightKeyboard)) {
		ship.rotatingRight = true;
	}
	else {
		ship.rotatingRight = false;
	}

	if (ctrl::IsKeyHeldDown(ship.reloadKey)) {
		ship::Reload(ship);
	}

	if (ctrl::IsMouseDown(ship.shootButton) || ctrl::IsKeyHeldDown(ship.shootKey)) {
		if (ship.timeSinceLastShot >= (3.0f * ship.fireRate / 4.0f)) {

			ship.shotQueued = true;
		}
	}
}

void ship::Update(ship::Ship& ship, prtcl::ParticleData fireParticles[])
{

	ship.timeSinceLastShot += rend::deltaTime;

	switch (ship.movementType)
	{
	case ship::MovementType::MOUSE: {
		ship.lookingAt = rend::mousePos - ship.pos;
		break;
	}
	case ship::MovementType::KEYBOARD: {
		ship.lookingAt = ship.direction.normalized() * ship.lookingAtDistance;
		if (ship.rotatingLeft) {
			ship.direction.rotate(ship.rotationSpeed * rend::deltaTime);
		}
		if (ship.rotatingRight) {
			ship.direction.rotate(-ship.rotationSpeed * rend::deltaTime);
		}
		break;
	}
	default: {

		break;
	}
	}

	ship.lookingAt.normalize();

	if (ship.braking) {
		ship.rotationSpeed = 5.0f;
		ship.speed -= ship.breakingPower;
		mth::Clamp(ship.speed, ship.minSpeed, ship.maxSpeed);
	}
	else {
		ship.rotationSpeed = 3.5f;
	}

	if (ship.accelerating) {
		ship.speed += ship.acceleration;
		mth::Clamp(ship.speed, ship.minSpeed, ship.maxSpeed);

		ship.direction += ship.lookingAt * ship.rotationSpeed * rend::deltaTime;
	}

	ship.direction.normalize();
	ship.direction = ship.direction * ship.speed * rend::deltaTime;
	ship.pos += ship.direction;

	if (ship.pos.x > 1.0f) {
		ship.pos.x = 0.0f;
	}
	if (ship.pos.x < 0.0f) {
		ship.pos.x = 1.0f;
	}
	if (ship.pos.y > 1.0f) {
		ship.pos.y = 0.0f;
	}
	if (ship.pos.y < 0.0f) {
		ship.pos.y = 1.0f;
	}

	if (ship.reloading) {

		ship.currentReloadTime += rend::deltaTime;

		if (ship.currentReloadTime >= ship.reloadTime) {
			ship.reloading = false;
			ship.shotQueued = false;
			ship.currentAmmoSize = ship.maxAmmoSize;
		}
	}


	ship.direction.normalize();
	ship.fireParticleActivator.pos = ship.pos - ship.direction * (ship.size.y / 2);
	ship.fireParticleActivator.direction = ship.direction.rotatedDegree(180);

	if (ship.accelerating) {
		ship.fireParticleActivator.speed = ship.fireParticleAcceleratingSpeed;
	}
	else {
		ship.fireParticleActivator.speed = ship.fireParticleNormalSpeed;
	}

	if (ship.braking) {
		ship.fireParticleActivator.spread = ship.fireParticleBreakingSpread;
		ship.fireParticleActivator.lifetime = ship.fireParticleBreakingLifetime;
	}
	else {
		ship.fireParticleActivator.spread = ship.fireParticleNormalSpread;
		ship.fireParticleActivator.lifetime = ship.fireParticleNormalLifetime;
	}

	prtcl::Update(ship.fireParticleActivator, fireParticles);
}

void ship::Draw(ship::Ship ship, prtcl::ParticleData fireParticles[])
{
	drw::Sprite(drw::spriteDataList[ship.textureID], ship.pos, ship.size);
	prtcl::Draw(fireParticles);

	if (ship.pos.x + ship.size.x/2.0f > 1.0f) {
		drw::Sprite(drw::spriteDataList[ship.textureID], { ship.pos.x - 1.0f,ship.pos.y }, ship.size);
	}
	if (ship.pos.x - ship.size.x / 2.0f < 0.0f) {
		drw::Sprite(drw::spriteDataList[ship.textureID], { ship.pos.x + 1.0f,ship.pos.y }, ship.size);
	}

	if (ship.pos.y + ship.size.y / 2.0f > 1.0f) {
		drw::Sprite(drw::spriteDataList[ship.textureID], { ship.pos.x,ship.pos.y - 1.0f }, ship.size);
	}
	if (ship.pos.y - ship.size.y / 2.0f < 0.0f) {
		drw::Sprite(drw::spriteDataList[ship.textureID], { ship.pos.x ,ship.pos.y + 1.0f }, ship.size);
	}
}

void ship::DrawHUD(Ship ship)
{
	drw::TextData text;
	text.fontID = 0;
	if (!ship.reloading) {
		std::string ammo = std::to_string(ship.currentAmmoSize);
		ammo.append("/");
		ammo.append(std::to_string(ship.maxAmmoSize));
		drw::Text(ammo.c_str(), text, { 0.075f,0.05f }, 75, {}, GREEN_B);
	}
	else {
		drw::Text("Reloading...", text, {0.075f,0.05f}, 50, {}, GREEN_B);

	}

}

bool ship::Reload(Ship& ship)
{
	if (ship.currentAmmoSize != ship.maxAmmoSize && !ship.reloading) {
		ship.reloading = true;
		ship.currentReloadTime = 0.0f;
		return true;
	}
	return false;
}

bool ship::Shoot(Ship& ship)
{
	if (ship.currentAmmoSize <= 0) {
		ship::Reload(ship);
		return false;
	}

	if (ship.shotQueued && !ship.reloading) {
		if (ship.timeSinceLastShot > ship.fireRate) {
			ship.timeSinceLastShot = 0.0f;
			ship.shotQueued = false;
			ship.currentAmmoSize--;
			return true;
		}
	}
	return false;
}
