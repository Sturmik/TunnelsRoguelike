#pragma once

#include "WindowManager.h"
#include "MapManager.h"

// Game manager controls flow of the game
class GameManager
{
private:
	// Window manager controls all window process
    WindowManager _windowManager;
	// Interface manager controls information showed on the screen
	InterfaceManager _interfaceManager;
	// Map manager controls map and objects on it
	MapManager _mapManager;

	// Update state of the game
	void UpdateState();
public:
	// Default constructor
	GameManager() : _mapManager(&_windowManager, &_interfaceManager) {}

	// Starts the game
	void Start();
};