#include "GameManager.h"

GameManager::GameManager() 
{
	// Initialize variables
}

void GameManager::Start()
{
	// Open window
	_windowManager.OpenWindow(sf::VideoMode(800, 600), "Tunnels");
	// Generate map
	MapGenerator mapGenerator(120, 60);
	mapGenerator.GenerateMap(8);
	for (int i = 0; i < mapGenerator.GetMapHeight(); i++)
	{
		for (int j = 0; j < mapGenerator.GetMapWidth(); j++)
		{
			_windowManager.AddObject(mapGenerator.GetMap()[i][j]);
		}
	}
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
	}
}