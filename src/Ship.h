#pragma once

#include "BorjaLib.h"

namespace ship {

	enum class MovementType {

		MOUSE,
		KEYBOARD
	};

	struct Ship {

		bool active = false;

		//Controls

		MovementType movementType = MovementType::MOUSE;

		ctrl::Key movementTypeChangeKey = ctrl::Key::TAB;

		ctrl::Key accelerateKeyboard = ctrl::Key::W;
		ctrl::Key rotateLeftKeyboard = ctrl::Key::A;
		ctrl::Key rotateRightKeyboard = ctrl::Key::D;

		ctrl::ButtonMouse accelerateButton = ctrl::ButtonMouse::RMB;
		
		ctrl::Key breakKey = ctrl::Key::LEFT_SHIFT;
		ctrl::Key shootKey = ctrl::Key::SPACE;
		ctrl::ButtonMouse shootButton = ctrl::ButtonMouse::LMB;
		
		ctrl::Key reloadKey = ctrl::Key::R;

		//Ship info

		vec::Vector2 size = { 0.02f,0.02f };

		float maxHealth = 3.0f;
		float health = maxHealth;

		float mass = 100.0f;

		float crashDamage = 1.0f;

		float damageReduction = 0.0f;

		int score = 0;
		int money = 0;

		//Movement

		vec::Vector2 pos = { 0.5f,0.5f };
		vec::Vector2 direction = { 0.0f,0.1f };
		vec::Vector2 lookingAt = { 0.0f,1.0f };

		float lookingAtDistance = 10.0f;

		float rotationSpeed = 3.5f;

		bool rotatingRight = false;
		bool rotatingLeft = false;

		float acceleration = 1.15f;
		float breakingPower = 1.1f;
		vec::Vector2 speed = { 0.0f, 0.01f };

		float minSpeed = 0.001f;
		float maxSpeed = 0.5f;

		bool accelerating = false;
		bool braking = false;

		//Shooting

		bool shotQueued = false;

		float maxShotAngle = 90.0f;

		int maxAmmoSize = 25;
		int currentAmmoSize = maxAmmoSize;

		//float bulletSpread = 20.0f;
		float bulletSpread = 0.0f;

		bool reloading = false;
		float reloadTime = 1.0f;
		float currentReloadTime = 0.0f;

		//float fireRate = 0.175f;
		float fireRate = 0.1f;
		float timeSinceLastShot = fireRate;

		//Textures

		int textureID = 0;

		//Particles

		prtcl::ParticleActivator fireParticleActivator;

		vec::Vector2 fireParticleNormalSpeed = { 0.005f,0.1f };
		vec::Vector2 fireParticleAcceleratingSpeed = { 0.06f,0.7f };

		vec::Vector2 fireParticleBreakingLifetime = { 0.1f,0.5f };
		vec::Vector2 fireParticleNormalLifetime = { 0.05f,0.25f };

		float fireParticleBreakingSpread = 120;
		float fireParticleNormalSpread = 45;
	};

	void Init(Ship& ship, drw::SpriteData& shipSprite, prtcl::ParticleData fireParticles[], drw::AnimationData fireAnim);

	void Reset(Ship& ship, prtcl::ParticleData fireParticles[]);
	
	void Input(Ship& ship);

	void Update(Ship& ship, prtcl::ParticleData fireParticles[]);

	void Draw(Ship ship, prtcl::ParticleData fireParticles[]);

	void DrawHUD(Ship ship);

	bool Reload(Ship& ship);

	bool Shoot(Ship& ship);

	void Move(Ship& ship);

	void MoveParticles(Ship& ship, prtcl::ParticleData fireParticles[]);
}