#include "GameManager.h"

GameManager::GameManager() 
{
	// Initialize variables
}

void GameManager::Start()
{
	// Open window
	_windowManager.OpenWindow(sf::VideoMode(1920, 1080), "Tunnels");
	// Generate map

	/// <summary>
	///  TEST
	//GameObject test;
	//sf::Texture toLoad; toLoad.loadFromFile("test.png");
	//test.SetTexture(toLoad);
	//test.setPosition(300, 300);
	//test.setScale(150, 150);
	//_windowManager.AddObject(Layer::FrontLayer, &test);
	/// </summary>

	Map map(120, 60);
	MapGenerator mapGenerator(&map);
	mapGenerator.GenerateMap(10);
	for (int i = 0; i < map.GetMapHeight(); i++)
	{
		for (int j = 0; j < map.GetMapWidth(); j++)
		{
			if (map.GetMap2D()[i][j].GetCellState() != CellState::None)
			{
				_windowManager.AddObject(Layer::BackLayer, &map.GetMap2D()[i][j]);
			}
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