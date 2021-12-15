#include "MapManager.h"

void MapManager::Update()
{
	// ...
}

void MapManager::GenerateMap(int numberOfRooms)
{
	// Generate map
	MapGenerator mapGenerator(&_map);
	mapGenerator.GenerateMap(numberOfRooms);
}

void MapManager::EventProcess(sf::Event windowEvent)
{
	switch (windowEvent.type)
	{
		// key pressed
	case sf::Event::KeyPressed:
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
			{
				// left key is pressed: move our character
				GenerateMap(5);
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
			{
				GenerateMap(10);
			}
	default:
		break;
	}
}