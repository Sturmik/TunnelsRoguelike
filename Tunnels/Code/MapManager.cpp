#include "MapManager.h"

void MapManager::Update()
{
}

void MapManager::NextMap(int numberOfRooms)
{
	// Generate map
	MapGenerator mapGenerator(&_map);
	mapGenerator.NextMap(numberOfRooms);

	// TEST

	_playerCharacter->RestoreTurnPoints();

	_playerCharacter->RemoveFromMapCell();
	_armorItem->RemoveFromMapCell();
	_weaponHolder->RemoveFromMapCell();
	_turnPotionHolder->RemoveFromMapCell();
	_healPotionHolder->RemoveFromMapCell();

	_playerCharacter->SetCellObjectPosition(Point2DInt(_map.GetRooms()[0].GetCenterOfTheRoomPosition().x,
	_map.GetRooms()[0].GetCenterOfTheRoomPosition().y));

	_armorItem->SetCellObjectPosition(Point2DInt(_map.GetRooms()[1].GetCenterOfTheRoomPosition().x,
		_map.GetRooms()[1].GetCenterOfTheRoomPosition().y));

	_weaponHolder->SetCellObjectPosition(Point2DInt(_map.GetRooms()[2].GetCenterOfTheRoomPosition().x,
		_map.GetRooms()[2].GetCenterOfTheRoomPosition().y));

	_turnPotionHolder->SetCellObjectPosition(Point2DInt(_map.GetRooms()[3].GetCenterOfTheRoomPosition().x,
		_map.GetRooms()[3].GetCenterOfTheRoomPosition().y));

	_healPotionHolder->SetCellObjectPosition(Point2DInt(_map.GetRooms()[4].GetCenterOfTheRoomPosition().x,
		_map.GetRooms()[4].GetCenterOfTheRoomPosition().y));

	_ladder->SetCellObjectPosition(Point2DInt(_map.GetRooms()[5].GetCenterOfTheRoomPosition().x,
		_map.GetRooms()[5].GetCenterOfTheRoomPosition().y));
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

	// TEST
	if (_playerCharacter->GetRecentTurnPoints() <= 0)
	{
		_playerCharacter->RestoreTurnPoints();
	}

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
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q))
		{
			_playerCharacter->UseRecentTurnPotion();
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

void MapManager::EventProcess(GameObject* gameObjectEvent)
{
	// Event can be called only by trigger object
	
	// Check, if it is a ladder - cast game object to ladder
	Ladder* ladder = dynamic_cast<Ladder*>(gameObjectEvent);
	if (ladder != nullptr)
	{
		// Generate new level
		NextMap(10);
	}
}