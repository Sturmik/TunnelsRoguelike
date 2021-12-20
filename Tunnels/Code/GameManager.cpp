#include "GameManager.h"

void GameManager::Start()
{
	// Open window
	_windowManager.OpenWindow(sf::VideoMode(500, 300), "Tunnels");
	// Start gameplay on map manager
	_mapManager.StartGameplay();
	// Start the game, by updating its state
	UpdateState();
}

void GameManager::UpdateState()
{
	// Infinite cycle
	while(true)
	{
		// Update window state
		_windowManager.Update();
		// Update map manager state
		_mapManager.Update();
	}
}