#pragma once

#include "WindowManager.h"
#include "MapGenerator.h"

// Game manager controls flow of the game
class GameManager
{
private:
	// Window manager controls all window process
    WindowManager _windowManager;

	// Update state of the game
	void UpdateState();
public:
	// Default constructor
	GameManager();

	// Starts the game
	void Start();
};