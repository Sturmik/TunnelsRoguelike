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


	// TEST
	_playerCharacter->SetCellObjectPosition(Point2DInt(_map.GetRooms()[0].GetRightBottomPosition().x - 1,
	_map.GetRooms()[0].GetRightBottomPosition().y - 1));

	_armorItem->SetCellObjectPosition(Point2DInt(_map.GetRooms()[1].GetRightBottomPosition().x - 1,
		_map.GetRooms()[1].GetRightBottomPosition().y - 1));

	_weaponHolder->SetCellObjectPosition(Point2DInt(_map.GetRooms()[2].GetRightBottomPosition().x - 1,
		_map.GetRooms()[2].GetRightBottomPosition().y - 1));

	_turnPotionHolder->SetCellObjectPosition(Point2DInt(_map.GetRooms()[3].GetRightBottomPosition().x - 1,
		_map.GetRooms()[3].GetRightBottomPosition().y - 1));

	_healPotionHolder->SetCellObjectPosition(Point2DInt(_map.GetRooms()[4].GetRightBottomPosition().x - 1,
		_map.GetRooms()[4].GetRightBottomPosition().y - 1));
}

void MapManager::WindowEventProcess(sf::RenderWindow& window, sf::Event windowEvent)
{
	MapCell* targetMapCell = nullptr;
	// Update view on player
	sf::View view = window.getView();
	view.move((_playerCharacter->getPosition().x - view.getCenter().x) * UtilityTime::GetDeltaTimeFloat(),
		(_playerCharacter->getPosition().y - view.getCenter().y) * UtilityTime::GetDeltaTimeFloat());
	window.setView(view);
	// Process events
	switch (windowEvent.type)
	{
		// Movement input check
	case sf::Event::KeyPressed:
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
		{
			_playerCharacter->MoveCellObject(PlacementDirection::Top, targetMapCell);
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
		{
			_playerCharacter->MoveCellObject(PlacementDirection::Bottom, targetMapCell);
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
		{
			_playerCharacter->MoveCellObject(PlacementDirection::Left, targetMapCell);
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
		{
			_playerCharacter->MoveCellObject(PlacementDirection::Right, targetMapCell);
		}
	default:
		break;
	}
	// Work with object
	if (targetMapCell != nullptr)
	{
		_playerCharacter->Interact(dynamic_cast<InteractiveObject*>(targetMapCell->GetGameObject()));
	}
}