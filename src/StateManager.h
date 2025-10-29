#pragma once

#include "BorjaLib.h"

#include "bulletManager.h"
#include "AsteroidManager.h"

enum class GameState {

	MAINMENU,
	GAMEPLAY,
	SETTINGS,
	CREDITS
};

enum class MMButtons
{
	PLAY,
	SETTINGS,
	CREDITS,
	EXIT,
	AMOUNT
};

enum class SButtons
{
	RENDERER,
	APPLY,
	EXIT,
	AMOUNT
};

enum class CButtons
{
	EXIT,
	AMOUNT
};

enum class PButtons
{
	PAUSE,
	RESUME,
	EXIT,
	AMOUNT
};

void GameLoop();
void InitMainMenuContext(btn::Container& container, btn::Button buttons[]);
void InitSettingsContext(btn::Container& container, btn::Button buttons[]);
void InitCreditsContext(btn::Container& container, btn::Button buttons[]);
void InitPauseContext(btn::Container& container, btn::Button buttons[]);