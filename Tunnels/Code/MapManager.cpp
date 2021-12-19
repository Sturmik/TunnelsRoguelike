#include "MapManager.h"

void MapManager::Update()
{
	// Map Manager updates configures turn cycle
	if (_turnState == CharacterType::PlayerType && _playerCharacter->GetRecentTurnPoints() <= 0)
	{
		// Pass turn to enemy
		_turnState = CharacterType::EnemyType;
		if (!_lightEnemyCharacter->IsObjectDead())
		{
			_lightEnemyCharacter->RestoreTurnPoints();
		}
		if (!_mediumEnemyCharacter->IsObjectDead())
		{
			_mediumEnemyCharacter->RestoreTurnPoints();
		}
		if (!_largeEnemyCharacter->IsObjectDead())
		{
			_largeEnemyCharacter->RestoreTurnPoints();
		}
	}
	if (_turnState == CharacterType::EnemyType &&
		_lightEnemyCharacter->GetRecentTurnPoints() <= 0 &&
		_mediumEnemyCharacter->GetRecentTurnPoints() <= 0 &&
		_largeEnemyCharacter->GetRecentTurnPoints() <= 0)
	{
		// Pass turn to player
		_turnState = CharacterType::PlayerType;
		_playerCharacter->RestoreTurnPoints();
	}
}

void MapManager::NextMap(int numberOfRooms)
{
	// Generate map
	MapGenerator mapGenerator(&_map);
	mapGenerator.NextMap(numberOfRooms);

	// TEST

	_turnState = CharacterType::PlayerType;

	_playerCharacter->RemoveFromMapCell();
	_armorItem->RemoveFromMapCell();
	_weaponHolder->RemoveFromMapCell();
	_turnPotionHolder->RemoveFromMapCell();
	_healPotionHolder->RemoveFromMapCell();

	_playerCharacter->SetCellObjectPosition(Point2DInt(_map.GetRooms()[0].GetCenterOfTheRoomPosition().x,
	_map.GetRooms()[0].GetCenterOfTheRoomPosition().y));

	_armorItem->SetCellObjectPosition(Point2DInt(_map.GetRooms()[5].GetCenterOfTheRoomPosition().x,
		_map.GetRooms()[5].GetCenterOfTheRoomPosition().y + 4));

	_weaponHolder->SetCellObjectPosition(Point2DInt(_map.GetRooms()[5].GetCenterOfTheRoomPosition().x,
		_map.GetRooms()[5].GetCenterOfTheRoomPosition().y + 3));

	_turnPotionHolder->SetCellObjectPosition(Point2DInt(_map.GetRooms()[5].GetCenterOfTheRoomPosition().x,
		_map.GetRooms()[5].GetCenterOfTheRoomPosition().y + 2));

	_healPotionHolder->SetCellObjectPosition(Point2DInt(_map.GetRooms()[5].GetCenterOfTheRoomPosition().x,
		_map.GetRooms()[5].GetCenterOfTheRoomPosition().y + 1));

	_lightEnemyCharacter->SetCellObjectPosition(Point2DInt(_map.GetRooms()[5].GetCenterOfTheRoomPosition().x + 1,
		_map.GetRooms()[5].GetCenterOfTheRoomPosition().y));

	_mediumEnemyCharacter->SetCellObjectPosition(Point2DInt(_map.GetRooms()[5].GetCenterOfTheRoomPosition().x,
		_map.GetRooms()[5].GetCenterOfTheRoomPosition().y));

	_largeEnemyCharacter->SetCellObjectPosition(Point2DInt(_map.GetRooms()[5].GetCenterOfTheRoomPosition().x - 1,
		_map.GetRooms()[5].GetCenterOfTheRoomPosition().y));

	_ladder->SetCellObjectPosition(Point2DInt(_map.GetRooms()[6].GetCenterOfTheRoomPosition().x,
		_map.GetRooms()[6].GetCenterOfTheRoomPosition().y));

	_playerCharacter->RestoreTurnPoints();
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