#include "BorjaLib.h"
#include "bulletManager.h"
#include "AsteroidManager.h"

int main() {

	bool isRunning = true;

	bLib::Init("Asteroids");

	//Sprites

	drw::SpriteData shipSprite;
	shipSprite.file = "res/sprites/noTexture.png";
	shipSprite.size = { 0.1f,0.1f };


	drw::SpriteData bulletSprite;
	bulletSprite.id = shipSprite.id;
	bulletSprite.size = { 0.01f,0.01f };

	drw::SpriteData asteroidSprite;
	asteroidSprite.id = shipSprite.id;
	asteroidSprite.size = { 0.2f,0.2f };

	drw::SpriteData starSprite;
	starSprite.id = shipSprite.id;
	starSprite.size = { 0.2f,0.2f };

	//Anim frames

	drw::SpriteData fireFrame1;
	fireFrame1.file = "res/sprites/fire1.png";
	fireFrame1.size = { 0.2f,0.2f };

	drw::SpriteData fireFrame2;
	fireFrame2.file = "res/sprites/fire2.png";
	fireFrame2.size = { 0.175f,0.175f };

	drw::SpriteData fireFrame3;
	fireFrame3.file = "res/sprites/fire3.png";
	fireFrame3.size = { 0.15f,0.15f };

	drw::SpriteData fireFrame4;
	fireFrame4.file = "res/sprites/fire4.png";
	fireFrame4.size = { 0.1f,0.1f };

	drw::SpriteData fireFrames[] = { fireFrame1,fireFrame2,fireFrame3, fireFrame4 };
	drw::SpriteData bulletFireFrames[] = { fireFrame1,fireFrame2,fireFrame3, fireFrame4 };
	drw::SpriteData asteroidExplosionFrames[] = { fireFrame1,fireFrame2,fireFrame3, fireFrame4 };
	drw::SpriteData starFrames[] = { fireFrame1,fireFrame1,fireFrame1, fireFrame1 };

	//Animations

	drw::AnimationData fireAnim;
	fireAnim.duration = 0.125f;

	drw::InitAnimData(fireAnim, fireFrames, 4);

	drw::AnimationData asteroidExplosionAnim;
	asteroidExplosionAnim.duration = 0.3f;

	drw::InitAnimData(asteroidExplosionAnim, asteroidExplosionFrames, 4);

	drw::AnimationData bulletFireAnim;
	bulletFireAnim.duration = 0.1f;

	drw::InitAnimData(bulletFireAnim, bulletFireFrames, 4);

	drw::AnimationData starAnim;
	starAnim.duration = 0.1f;

	drw::InitAnimData(starAnim, starFrames, 4);

	// Particles

	prtcl::ParticleData fireParticle;

	prtcl::ParticleData fireParticles[] = { fireParticle ,fireParticle ,fireParticle ,fireParticle ,fireParticle,fireParticle,fireParticle,fireParticle,fireParticle,fireParticle,fireParticle,fireParticle,fireParticle,fireParticle,fireParticle };

	prtcl::ParticleData fireBulletParticle;

	prtcl::ParticleData asteroidParticles[] = { fireBulletParticle ,fireBulletParticle ,fireBulletParticle };

	prtcl::ParticleData bulletParticles[] = { fireBulletParticle ,fireBulletParticle ,fireBulletParticle };

	prtcl::ParticleData starParticles[] = { fireParticle ,fireParticle ,fireParticle ,fireParticle ,fireParticle,fireParticle,fireParticle,fireParticle,fireParticle,fireParticle,fireParticle,fireParticle,fireParticle,fireParticle,fireParticle,fireParticle ,fireParticle ,fireParticle ,fireParticle ,fireParticle,fireParticle,fireParticle,fireParticle,fireParticle,fireParticle,fireParticle,fireParticle,fireParticle,fireParticle,fireParticle };


	//Background

	prtcl::ParticleActivator backgroundStars;
	backgroundStars.amount = 30;
	backgroundStars.animated = true;
	backgroundStars.animation = starAnim;
	backgroundStars.loop = true;
	backgroundStars.delay = { 0.5f,7.5f };
	backgroundStars.direction = { 0.0f,1.0f };
	backgroundStars.spread = 360.0f;
	backgroundStars.speed = { 0.0001f,0.0002f };
	backgroundStars.lifetime = { 7.5f,20.0f };
	backgroundStars.startingOffset = { 1.0f,1.0f };
	backgroundStars.pos = { 0.5f, 0.5f };
	backgroundStars.startingPosInfluence = 0.0f;
	backgroundStars.minSize = {0.002f,0.002f};
	backgroundStars.maxSize = {0.004f,0.004f};
	prtcl::Init(backgroundStars,starParticles);
	
	//Asteroids

	asteroidManager::AsteroidSpawner asteroidSpawner;

	asteroidManager::Init(asteroidSpawner);

	asteroid::Asteroid asteroids[asteroidManager::maxAsteroidAmount] = {};

	asteroid::Init(asteroids, asteroidManager::maxAsteroidAmount, asteroidSprite, asteroidParticles, asteroidExplosionAnim);
	
	//Bullets

	bullet::Bullet bullets[bulletManager::maxBulletAmount] = {};

	bullet::Init(bullets,bulletManager::maxBulletAmount,bulletSprite,bulletParticles, bulletFireAnim);

	//Ship

	ship::Ship ship;
	
	ship::Init(ship,shipSprite,fireParticles,fireAnim);

	while (isRunning) {

		isRunning = !rend::ShouldExit();

		//update
		bLib::UpdateStart();

		asteroidManager::Update(asteroidSpawner,asteroids,asteroidManager::maxAsteroidAmount);

		ship::Input(ship);

		ship::Update(ship,fireParticles);

		if (ship::Shoot(ship)) {
			bulletManager::Shoot(ship, bullets, bulletManager::maxBulletAmount);
		}

		bullet::Update(bullets,bulletManager::maxBulletAmount);

		asteroid::Update(asteroids,asteroidManager::maxAsteroidAmount);

		prtcl::Update(backgroundStars,starParticles);

		bLib::UpdateEnd();

		//Outputs

		//draw
		drw::Begin();
		drw::Clear(BLACK_B);

		prtcl::Draw(starParticles);

		asteroid::Draw(asteroids,asteroidManager::maxAsteroidAmount);

		bullet::Draw(bullets,bulletManager::maxBulletAmount);

		ship::Draw(ship,fireParticles);

		ship::DrawHUD(ship);

		drw::End();

		//sounds
	}
	rend::Close();
	return 0;
}
