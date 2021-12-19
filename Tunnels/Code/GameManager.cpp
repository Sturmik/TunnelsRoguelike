#include "GameManager.h"

void GameManager::Start()
{
	// Open window
	_windowManager.OpenWindow(sf::VideoMode(500, 300), "Tunnels");
	// Generate map
	_mapManager.NextMap(10);
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