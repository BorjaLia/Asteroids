#include "Bullet.h"

void bullet::Init(Bullet& bullet, drw::SpriteData& bulletSprite, prtcl::ParticleData bulletParticles[], drw::AnimationData bulletFireAnim)
{
	bullet.textureID = bulletSprite.id;

	bullet.bulletParticleActivator.animation = bulletFireAnim;

	bullet.bulletParticleActivator.loop = true;
	bullet.bulletParticleActivator.id = 0;
	bullet.bulletParticleActivator.animated = true;
	bullet.bulletParticleActivator.loop = true;
	bullet.bulletParticleActivator.amount = bullet::particlesAmount;
	bullet.bulletParticleActivator.lifetime = { 0.05f,0.075f };
	bullet.bulletParticleActivator.direction = { 0.1f,1.0f };
	bullet.bulletParticleActivator.spread = 45.0f;
	bullet.bulletParticleActivator.minSize = { 0.0005f,0.0005f };
	bullet.bulletParticleActivator.maxSize = { 0.0025f,0.0025f };
	bullet.bulletParticleActivator.pos = bullet.pos;
	bullet.bulletParticleActivator.delay = { 0.0f,0.125f };
	bullet.bulletParticleActivator.speed = { 0.005f,0.1f };

	bullet::Reset(bullet);

	prtcl::Init(bullet.bulletParticleActivator, bulletParticles);
	
	for (int i = 0; i < particlesAmount; i++)
	{
		bullet.particles[i] = bulletParticles[i];
	}

	bullet.size.x /= rend::windowRatio;
	bullet.pos.x /= rend::windowRatio;
	bullet.direction.x /= rend::windowRatio;
}

void bullet::Init(Bullet bullets[],int bulletAmount, drw::SpriteData& bulletSprite, prtcl::ParticleData bulletParticles[], drw::AnimationData bulletFireAnim)
{
	for (int i = 0; i < bulletAmount; i++)
	{
		bullets[i] = bullet::Bullet();
		bullet::Init(bullets[i],bulletSprite,bulletParticles,bulletFireAnim);
	}
}

void bullet::Reset(Bullet& bullet)
{
	bullet.pos = bullet::Bullet().pos;

	for (int i = 0; i < particlesAmount; i++)
	{
		bullet.particles[i].pos = bullet.pos;
	}

	bullet.active = false;
}

void bullet::Reset(Bullet bullets[], int bulletAmount)
{
	for (int i = 0; i < bulletAmount; i++)
	{
		bullet::Reset(bullets[i]);
	}
}

void bullet::Activate(Bullet& bullet)
{
	bullet.active = true;
	bullet.speed = bullet.startingSpeed;

	bullet.currentLifetime = 0.0f;
}

void bullet::Update(Bullet& bullet)
{
	if (!bullet.active) {
		return;
	}

	bullet.speed -= bullet.deceleration;

	bullet.direction.normalize();
	bullet.direction = bullet.direction * bullet.speed * rend::deltaTime;
	bullet.pos += bullet.direction;

	if (bullet.pos.x > 1.0f) {
		bullet.pos.x = 0.0f;
	}
	if (bullet.pos.x < 0.0f) {
		bullet.pos.x = 1.0f;
	}

	if (bullet.pos.y > 1.0f) {
		bullet.pos.y = 0.0f;
	}
	if (bullet.pos.y < 0.0f) {
		bullet.pos.y = 1.0f;
	}

	bullet.currentLifetime += rend::deltaTime;

	bullet.bulletParticleActivator.pos = bullet.pos - bullet.direction * (bullet.size.y / 2);
	bullet.bulletParticleActivator.direction = bullet.direction.rotatedDegree(180);

	prtcl::Update(bullet.bulletParticleActivator,bullet.particles);

	if (bullet.currentLifetime >= bullet.maxLifetime || bullet.speed <= bullet.minSpeed) {
		bullet::Reset(bullet);
	}
}

void bullet::Update(Bullet bullets[], int bulletAmount)
{
	for (int i = 0; i < bulletAmount; i++)
	{
		bullet::Update(bullets[i]);
	}
}

void bullet::Draw(Bullet bullet)
{
	drw::Sprite(drw::spriteDataList[bullet.textureID], bullet.pos, bullet.size);
	for (int i = 0; i < 3; i++)
	{
		prtcl::Draw(bullet.particles);
	}

	if (bullet.pos.x + bullet.size.x > 1.0f) {
		drw::Sprite(drw::spriteDataList[bullet.textureID], { bullet.pos.x - 1.0f,bullet.pos.y }, bullet.size);
	}
	if (bullet.pos.x - bullet.size.x < 0.0f) {
		drw::Sprite(drw::spriteDataList[bullet.textureID], { bullet.pos.x + 1.0f,bullet.pos.y }, bullet.size);
	}

	if (bullet.pos.y + bullet.size.y > 1.0f) {
		drw::Sprite(drw::spriteDataList[bullet.textureID], { bullet.pos.x,bullet.pos.y - 1.0f }, bullet.size);
	}
	if (bullet.pos.y - bullet.size.y < 0.0f) {
		drw::Sprite(drw::spriteDataList[bullet.textureID], { bullet.pos.x ,bullet.pos.y + 1.0f }, bullet.size);
	}
	if (rend::devInfo != rend::InfoMode::NONE) {
		drw::Circle(bullet.pos, bullet.size, BLUE_B);
	}
}

void bullet::Draw(Bullet bullets[], int bulletAmount)
{
	for (int i = 0; i < bulletAmount; i++)
	{
		bullet::Draw(bullets[i]);
	}
}
