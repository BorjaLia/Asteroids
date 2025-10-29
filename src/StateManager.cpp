#include "StateManager.h"

void GameLoop()
{
	bool isRunning = true;

	bool paused = false;
	bool lost = false;

	GameState gameState = GameState::MAINMENU;

	std::string settingsFilePath = "config/Settings.txt";

	bLib::Init("Asteroids");

	rend::GraphicsLib graphics = rend::activeGraphics;

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
	backgroundStars.minSize = { 0.002f,0.002f };
	backgroundStars.maxSize = { 0.004f,0.004f };
	prtcl::Init(backgroundStars, starParticles);


	//Buttons

	btn::Container mainMenuContainer;

	btn::Button playButton;
	btn::Button settingsButton;
	btn::Button creditsButton;
	btn::Button exitButton;

	btn::Button mainMenuButtons[] = { playButton, settingsButton, creditsButton, exitButton };

	InitMainMenuContext(mainMenuContainer, mainMenuButtons);

	//mainMenuButtons[static_cast<int>(MMButtons::SETTINGS)].isActive = false;
	//mainMenuButtons[static_cast<int>(MMButtons::SETTINGS)].isRendered = false;
	//mainMenuButtons[static_cast<int>(MMButtons::CREDITS)].pos.y += (mainMenuButtons[static_cast<int>(MMButtons::SETTINGS)].pos.y - mainMenuButtons[static_cast<int>(MMButtons::CREDITS)].pos.y)/2.0f;

	btn::Container settingsContainer;

	btn::Button rendererButton;
	btn::Button applyButton;

	btn::Button settingsButtons[] = { rendererButton,applyButton, exitButton };

	InitSettingsContext(settingsContainer, settingsButtons);

	btn::Container creditsContainer;

	btn::Button nameButton;

	btn::Button creditsButtons[] = { nameButton, exitButton };

	InitCreditsContext(creditsContainer, creditsButtons);

	btn::Container pauseContainer;

	btn::Button pauseButton;
	btn::Button resumeButton;

	btn::Button pauseButtons[] = { pauseButton,resumeButton, exitButton };

	InitPauseContext(pauseContainer, pauseButtons);

	//Asteroids

	asteroidManager::AsteroidSpawner asteroidSpawner;

	asteroidManager::Init(asteroidSpawner);

	asteroid::Asteroid asteroids[asteroidManager::maxAsteroidAmount] = {};

	asteroid::Init(asteroids, asteroidManager::maxAsteroidAmount, asteroidSprite, asteroidParticles, asteroidExplosionAnim);

	//Bullets

	bullet::Bullet bullets[bulletManager::maxBulletAmount] = {};

	bullet::Init(bullets, bulletManager::maxBulletAmount, bulletSprite, bulletParticles, bulletFireAnim);

	//Ship

	ship::Ship ship;

	ship::Init(ship, shipSprite, fireParticles, fireAnim);

	while (isRunning) {

		isRunning = !rend::ShouldExit();

		//update
		bLib::UpdateStart();
		if (!paused && !lost) {
			prtcl::Update(backgroundStars, starParticles);
		}

		switch (gameState)
		{
		case GameState::MAINMENU: {
			btn::UpdateInput(mainMenuButtons, static_cast<int>(MMButtons::AMOUNT));

			if (mainMenuButtons[static_cast<int>(MMButtons::PLAY)].signal) {
				gameState = GameState::GAMEPLAY;
				pauseButtons[static_cast<int>(PButtons::PAUSE)].isActive = true;
				pauseButtons[static_cast<int>(PButtons::PAUSE)].isRendered = true;
				pauseButtons[static_cast<int>(PButtons::RESUME)].isActive = false;
				pauseButtons[static_cast<int>(PButtons::RESUME)].isRendered = false;
				pauseButtons[static_cast<int>(PButtons::EXIT)].isActive = false;
				pauseButtons[static_cast<int>(PButtons::EXIT)].isRendered = false;

				ship::Reset(ship,fireParticles);
				bullet::Reset(bullets,bulletManager::maxBulletAmount);
				asteroid::Reset(asteroids, asteroidManager::maxAsteroidAmount);

				asteroidManager::Reset(asteroidSpawner);
				bulletManager::Reset();

				paused = false;
				lost = false;
			}
			if (mainMenuButtons[static_cast<int>(MMButtons::SETTINGS)].signal) {
				gameState = GameState::SETTINGS;
			}
			if (mainMenuButtons[static_cast<int>(MMButtons::CREDITS)].signal) {
				gameState = GameState::CREDITS;
			}
			if (mainMenuButtons[static_cast<int>(MMButtons::EXIT)].signal) {
				isRunning = false;

			}

			break;
		}
		case GameState::GAMEPLAY: {

			btn::UpdateInput(pauseButtons, static_cast<int>(PButtons::AMOUNT));

			if (!paused && !lost) {

				if (pauseButtons[static_cast<int>(PButtons::PAUSE)].signal || ctrl::IsKeyPressed(ctrl::Key::ESCAPE)) {
					paused = true;
					pauseButtons[static_cast<int>(PButtons::PAUSE)].isActive = false;
					pauseButtons[static_cast<int>(PButtons::PAUSE)].isRendered = false;
					pauseButtons[static_cast<int>(PButtons::RESUME)].isActive = true;
					pauseButtons[static_cast<int>(PButtons::RESUME)].isRendered = true;
					pauseButtons[static_cast<int>(PButtons::EXIT)].isActive = true;
					pauseButtons[static_cast<int>(PButtons::EXIT)].isRendered = true;
				}

				asteroidManager::Update(asteroidSpawner, ship, bullets, bulletManager::maxBulletAmount, asteroids, asteroidManager::maxAsteroidAmount);

				ship::Input(ship);

				ship::Update(ship, fireParticles);

				if (ship::Shoot(ship)) {
					bulletManager::Shoot(ship, bullets, bulletManager::maxBulletAmount);
				}

				bullet::Update(bullets, bulletManager::maxBulletAmount);

				asteroid::Update(asteroids, asteroidManager::maxAsteroidAmount);

				if (ship.health <= 0.0f) {
					lost = true;
					pauseButtons[static_cast<int>(PButtons::PAUSE)].isActive = false;
					pauseButtons[static_cast<int>(PButtons::PAUSE)].isRendered = false;
					pauseButtons[static_cast<int>(PButtons::RESUME)].isActive = false;
					pauseButtons[static_cast<int>(PButtons::RESUME)].isRendered = false;
					pauseButtons[static_cast<int>(PButtons::EXIT)].isActive = true;
					pauseButtons[static_cast<int>(PButtons::EXIT)].isRendered = true;
				}

			}
			else if (paused && !lost){
				if (pauseButtons[static_cast<int>(PButtons::RESUME)].signal || ctrl::IsKeyPressed(ctrl::Key::ESCAPE)) {
					paused = false;
					pauseButtons[static_cast<int>(PButtons::PAUSE)].isActive = true;
					pauseButtons[static_cast<int>(PButtons::PAUSE)].isRendered = true;
					pauseButtons[static_cast<int>(PButtons::RESUME)].isActive = false;
					pauseButtons[static_cast<int>(PButtons::RESUME)].isRendered = false;
					pauseButtons[static_cast<int>(PButtons::EXIT)].isActive = false;
					pauseButtons[static_cast<int>(PButtons::EXIT)].isRendered = false;
				}
				if (pauseButtons[static_cast<int>(PButtons::EXIT)].signal) {
					paused = false;
					lost = false;
					gameState = GameState::MAINMENU;
				}
			}
			else if (lost) {
				if (pauseButtons[static_cast<int>(PButtons::EXIT)].signal) {
					paused = false;
					lost = false;
					gameState = GameState::MAINMENU;
				}
			}

			if (ctrl::IsKeyPressed(ctrl::Key::F4)) {
				std::cout << '\n';
				std::cout << '\n';
				std::cout << '\n';
				std::cout << '\n';
				std::cout << '\n';
				std::cout << "SPACING FRAME N:" << rend::frameCounter << '\n';
				std::cout << '\n';
				std::cout << '\n';
				std::cout << '\n';
				std::cout << '\n';
			}

			if (ctrl::IsKeyPressed(ctrl::Key::F5)) {
				std::cout << '\n';
				std::cout << "Ship:" << '\n';
				std::cout << "Hp:" << ship.health << '\n';
				std::cout << "Money:" << ship.money << '\n';
				std::cout << '\n';
			}
			break;
		}
		case GameState::SETTINGS: {
			btn::UpdateInput(settingsButtons, static_cast<int>(SButtons::AMOUNT));

			if (settingsButtons[static_cast<int>(SButtons::RENDERER)].signal) {
				switch (graphics)
				{
				case rend::GraphicsLib::RAYLIB: {
					graphics = rend::GraphicsLib::SIGIL;
					settingsButtons[static_cast<int>(SButtons::RENDERER)].textData.text = "Sigil";
					break;
				}
				case rend::GraphicsLib::SIGIL: {
					graphics = rend::GraphicsLib::RAYLIB;
					settingsButtons[static_cast<int>(SButtons::RENDERER)].textData.text = "Raylib";
					break;
				}
				default:
					break;
				}
			}

			if (settingsButtons[static_cast<int>(SButtons::APPLY)].signal) {
				utl::SaveToFile(settingsFilePath.c_str(), "graphics", static_cast<int>(graphics));
				gameState = GameState::MAINMENU;
			}

			if (settingsButtons[static_cast<int>(SButtons::EXIT)].signal) {
				gameState = GameState::MAINMENU;
				graphics = rend::activeGraphics;
			}
			break;
		}
		case GameState::CREDITS: {

			btn::UpdateInput(creditsButtons, static_cast<int>(CButtons::AMOUNT));

			if (creditsButtons[static_cast<int>(CButtons::EXIT)].signal) {
				gameState = GameState::MAINMENU;
			}

			break;
		}
		default:
			break;
		}


		bLib::UpdateEnd();

		//Outputs

		//draw
		drw::Begin();
		drw::Clear(BLACK_B);

		prtcl::Draw(starParticles);

		switch (gameState)
		{
		case GameState::MAINMENU: {
			btn::Draw(mainMenuContainer, mainMenuButtons, static_cast<int>(MMButtons::AMOUNT));
			break;
		}
		case GameState::GAMEPLAY: {
			asteroid::Draw(asteroids, asteroidManager::maxAsteroidAmount);

			bullet::Draw(bullets, bulletManager::maxBulletAmount);

			ship::Draw(ship, fireParticles);

			ship::DrawHUD(ship);

			if (paused || lost) {
				drw::Rectangle({ 0.5f,0.5f }, {1.0f, 1.0f},SEMITRANSPARENT_B);
			}

			btn::Draw(pauseContainer, pauseButtons, static_cast<int>(PButtons::AMOUNT));
			
			if (lost) {

				drw::Text("You lost!", { 0.5f,0.65f }, 80, {0.0f,0.0f},MAROON_B);
				drw::Text("score:", { 0.5f,0.5f }, 60, {0.0f,0.0f},WHITE_B);
				drw::Text(std::to_string(ship.score).c_str(), {0.5f,0.45f}, 50, {0.0f,0.0f}, GREEN_B);
			}
			break;
		}
		case GameState::SETTINGS: {

			drw::Text("Settings", { 0.5f,0.7f }, 80);
			btn::Draw(settingsContainer, settingsButtons, static_cast<int>(SButtons::AMOUNT));

			break;
		}
		case GameState::CREDITS: {
			btn::Draw(creditsContainer, creditsButtons, static_cast<int>(CButtons::AMOUNT));

			drw::Text("Made by", { 0.5f,0.7f }, 70);
			drw::Text("Borja Lia", { 0.5f,0.6f }, 120);

			break;
		}
		default:
			break;
		}
		drw::End();

		//sounds

		switch (gameState)
		{
		case GameState::MAINMENU: {

			break;
		}
		case GameState::GAMEPLAY: {

			break;
		}
		case GameState::SETTINGS: {

			break;
		}
		case GameState::CREDITS: {

			break;
		}
		default:
			break;
		}
	}
	rend::Close();
}

void InitMainMenuContext(btn::Container& container, btn::Button buttons[])
{
	container.pos = { 0.5f,0.5f };
	container.size = { 0.25f,0.75f };
	container.isRendered = false;
	container.useTexture = false;

	btn::Init(container);

	for (int b = 0; b < static_cast<int>(MMButtons::AMOUNT); b++)
	{
		buttons[b].signalTimerLimit = 0.1f;
		buttons[b].size = { 0.2f,0.1f };
		buttons[b].pos = { 0.5f, 0.5f };
		buttons[b].clickedOffset = { 0.0f,-0.1f };
		buttons[b].useText = true;
		buttons[b].useSprite = false;
		buttons[b].isActive = true;
		buttons[b].textData = drw::TextData();
		buttons[b].activeColor = TRANSPARENT_B;
		buttons[b].mainColor = TRANSPARENT_B;
		buttons[b].hoveredColor = TRANSPARENT_B;
		buttons[b].textColor = WHITE_B;
		buttons[b].activeTextureID = 0;
		buttons[b].mainTextureID = 0;
		buttons[b].hoveredTextureID = 0;
	}

	buttons[static_cast<int>(MMButtons::PLAY)].textData.text = "Play";
	buttons[static_cast<int>(MMButtons::PLAY)].pos.y = 0.6f;

	btn::Init(buttons[static_cast<int>(MMButtons::PLAY)], container, true);

	buttons[static_cast<int>(MMButtons::SETTINGS)].textData.text = "Settings";
	buttons[static_cast<int>(MMButtons::SETTINGS)].pos.y = 0.45f;

	btn::Init(buttons[static_cast<int>(MMButtons::SETTINGS)], container, true);

	buttons[static_cast<int>(MMButtons::CREDITS)].textData.text = "Credits";
	buttons[static_cast<int>(MMButtons::CREDITS)].pos.y = 0.3f;

	btn::Init(buttons[static_cast<int>(MMButtons::CREDITS)], container, true);

	buttons[static_cast<int>(MMButtons::EXIT)].textData.text = "Exit";
	buttons[static_cast<int>(MMButtons::EXIT)].pos.y = 0.15f;

	btn::Init(buttons[static_cast<int>(MMButtons::EXIT)], container, true);

}

void InitSettingsContext(btn::Container& container, btn::Button buttons[])
{
	container.pos = { 0.5f,0.5f };
	container.size = { 0.5f,1.0f };
	container.isRendered = false;
	container.useTexture = false;

	btn::Init(container);

	for (int b = 0; b < static_cast<int>(SButtons::AMOUNT); b++)
	{
		buttons[b].signalTimerLimit = 0.1f;
		buttons[b].size = { 0.2f,0.1f };
		buttons[b].pos = { 0.5f, 0.5f };
		buttons[b].clickedOffset = { 0.0f,-0.1f };
		buttons[b].useText = true;
		buttons[b].useSprite = false;
		buttons[b].isActive = true;
		buttons[b].textData = drw::TextData();
		buttons[b].activeColor = TRANSPARENT_B;
		buttons[b].mainColor = TRANSPARENT_B;
		buttons[b].hoveredColor = TRANSPARENT_B;
		buttons[b].textColor = WHITE_B;
		buttons[b].activeTextureID = 0;
		buttons[b].mainTextureID = 0;
		buttons[b].hoveredTextureID = 0;
	}

	buttons[static_cast<int>(SButtons::RENDERER)].textData.text = "Renderer";
	
	if (rend::activeGraphics == rend::GraphicsLib::RAYLIB) {
		buttons[static_cast<int>(SButtons::RENDERER)].textData.text = "Raylib";
	}
	else if (rend::activeGraphics == rend::GraphicsLib::SIGIL) {
		buttons[static_cast<int>(SButtons::RENDERER)].textData.text = "Sigil";
	}
	buttons[static_cast<int>(SButtons::RENDERER)].pos.y = 0.5f;

	btn::Init(buttons[static_cast<int>(SButtons::RENDERER)], container, true);

	buttons[static_cast<int>(SButtons::APPLY)].textData.text = "Apply";
	buttons[static_cast<int>(SButtons::APPLY)].pos = { 0.75f, 0.15f };

	btn::Init(buttons[static_cast<int>(SButtons::APPLY)], container, true);

	buttons[static_cast<int>(SButtons::EXIT)].textData.text = "Exit";
	buttons[static_cast<int>(SButtons::EXIT)].pos = { 0.25f,0.15f };

	btn::Init(buttons[static_cast<int>(SButtons::EXIT)], container, true);

}

void InitPauseContext(btn::Container& container, btn::Button buttons[])
{
	container.pos = { 0.5f,0.5f };
	container.size = { 0.25f,1.0f };
	container.isRendered = false;
	container.useTexture = false;

	btn::Init(container);

	for (int b = 0; b < static_cast<int>(PButtons::AMOUNT); b++)
	{
		buttons[b].signalTimerLimit = 0.1f;
		buttons[b].size = { 0.2f,0.1f };
		buttons[b].pos = { 0.5f, 0.5f };
		buttons[b].clickedOffset = { 0.0f,-0.1f };
		buttons[b].useText = true;
		buttons[b].useSprite = false;
		buttons[b].isActive = true;
		buttons[b].textData = drw::TextData();
		buttons[b].activeColor = TRANSPARENT_B;
		buttons[b].mainColor = TRANSPARENT_B;
		buttons[b].hoveredColor = TRANSPARENT_B;
		buttons[b].textColor = WHITE_B;
		buttons[b].activeTextureID = 0;
		buttons[b].mainTextureID = 0;
		buttons[b].hoveredTextureID = 0;
	}

	buttons[static_cast<int>(PButtons::PAUSE)].textData.text = "Pause";
	buttons[static_cast<int>(PButtons::PAUSE)].pos.y = 0.95f;

	btn::Init(buttons[static_cast<int>(PButtons::PAUSE)], container, true);

	buttons[static_cast<int>(PButtons::RESUME)].textData.text = "Resume";
	buttons[static_cast<int>(PButtons::RESUME)].pos.y = 0.5f;

	btn::Init(buttons[static_cast<int>(PButtons::RESUME)], container, true);

	buttons[static_cast<int>(PButtons::EXIT)].textData.text = "Exit";
	buttons[static_cast<int>(PButtons::EXIT)].pos.y = 0.35f;

	btn::Init(buttons[static_cast<int>(PButtons::EXIT)], container, true);
}

void InitCreditsContext(btn::Container& container, btn::Button buttons[])
{
	container.pos = { 0.5f,0.5f };
	container.size = { 0.25f,0.75f };
	container.isRendered = false;
	container.useTexture = false;

	btn::Init(container);

	for (int b = 0; b < static_cast<int>(CButtons::AMOUNT); b++)
	{
		buttons[b].signalTimerLimit = 0.1f;
		buttons[b].size = { 0.2f,0.1f };
		buttons[b].pos = { 0.5f, 0.5f };
		buttons[b].clickedOffset = { 0.0f,-0.1f };
		buttons[b].useText = true;
		buttons[b].useSprite = false;
		buttons[b].isActive = true;
		buttons[b].textData = drw::TextData();
		buttons[b].activeColor = TRANSPARENT_B;
		buttons[b].mainColor = TRANSPARENT_B;
		buttons[b].hoveredColor = TRANSPARENT_B;
		buttons[b].textColor = WHITE_B;
		buttons[b].activeTextureID = 0;
		buttons[b].mainTextureID = 0;
		buttons[b].hoveredTextureID = 0;
	}

	buttons[static_cast<int>(CButtons::EXIT)].textData.text = "Exit";
	buttons[static_cast<int>(CButtons::EXIT)].pos.y = 0.15f;

	btn::Init(buttons[static_cast<int>(CButtons::EXIT)], container, true);
}
