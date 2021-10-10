#pragma once

#include "WindowManager.h"

// Game manager controls flow of the game
class GameManager
{
private:
#pragma region Variables
	// Window manager controls all window process
	WindowManager _windowManager;
#pragma endregion
#pragma region Methods
	// Update state of game
	void UpdateState();
#pragma endregion
public:
#pragma region Constructor
	GameManager();
#pragma endregion
#pragma region Methods
	// Starts the game
	void Start();
#pragma endregion
};