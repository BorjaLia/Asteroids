#include "BorjaLib.h"
#include "BulletManager.h"

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

	//Animations

	drw::AnimationData fireAnim;
	fireAnim.duration = 0.125f;

	drw::InitAnimData(fireAnim, fireFrames, 4);

	drw::AnimationData bulletFireAnim;
	bulletFireAnim.duration = 0.1f;

	drw::InitAnimData(bulletFireAnim, bulletFireFrames, 4);

	// Particles

	prtcl::ParticleData fireParticle;

	prtcl::ParticleData fireParticles[] = { fireParticle ,fireParticle ,fireParticle ,fireParticle ,fireParticle,fireParticle,fireParticle,fireParticle,fireParticle,fireParticle,fireParticle,fireParticle,fireParticle,fireParticle,fireParticle };
	
	prtcl::ParticleData fireBulletParticle;

	prtcl::ParticleData bulletParticles[] = { fireBulletParticle ,fireBulletParticle ,fireBulletParticle };


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

		ship::Input(ship);

		ship::Update(ship,fireParticles);



		if (ship::Shoot(ship)) {
			bulletManager::Shoot(ship, bullets, bulletManager::maxBulletAmount);
		}

		bullet::Update(bullets,bulletManager::maxBulletAmount);

		bLib::UpdateEnd();

		//Outputs

		//draw
		drw::Begin();
		drw::Clear(BLACK_B);

		bullet::Draw(bullets,bulletManager::maxBulletAmount);
		ship::Draw(ship,fireParticles);

		ship::DrawHUD(ship);

		drw::End();

		//sounds
	}
	rend::Close();
	return 0;
}
