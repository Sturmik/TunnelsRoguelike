#include "MapManager.h"

MapManager::~MapManager()
{
	// Delete game objects and remove them from window manage
	delete _playerCharacter;
	_windowManager->RemoveObject(_playerCharacter);
	delete _ladder;
	_windowManager->RemoveObject(_ladder);
	for (std::list<EnemyCharacter*>::iterator it = _enemyCharacterList.begin();
		it != _enemyCharacterList.end(); it++)
	{
		// Remove object from window manager
		_windowManager->RemoveObject(*it);
		// Delete object
		delete* it;
	}
	for (std::list<Item*>::iterator it = _itemList.begin();
		it != _itemList.end(); it++)
	{
		// Remove object from window manager
		_windowManager->RemoveObject(*it);
		// Delete object
		delete* it;
	}
	// Unsubscribe from window
	_windowManager->RemoveListener(this);
}

void MapManager::Update()
{
	// If player has died - declare game over 
	if (_playerCharacter->IsObjectDead()) { GameOver(); }
	// Map Manager updates configures turn cycle
	if (_turnState == CharacterType::PlayerType && _playerCharacter->GetRecentTurnPoints() <= 0)
	{
		// Pass turn to enemy
		_turnState = CharacterType::EnemyType;
		// Restore enemy turn points
		for (std::list<EnemyCharacter*>::iterator it = _enemyCharacterList.begin();
			it != _enemyCharacterList.end(); it++)
		{
			// If enemy is alive - restore its turn points
			if (!(**it).IsObjectDead()) { (**it).RestoreTurnPoints(); }
		}
	}
	if (_turnState == CharacterType::EnemyType)
	{
		bool hasEnemyTurnEnded = true;
		for (std::list<EnemyCharacter*>::iterator it = _enemyCharacterList.begin();
			it != _enemyCharacterList.end(); it++)
		{
			// If enemy is dead - skip it
			if ((**it).IsObjectDead()) { continue; }
			// If enemy still has turn points - break cycle and mark boolean variable as false
			if ((**it).GetRecentTurnPoints() > 0) { hasEnemyTurnEnded = false; break; }
		}
		// If all enemies used their turn points
		if (hasEnemyTurnEnded)
		{
			// Pass turn to player
			_turnState = CharacterType::PlayerType;
			_playerCharacter->RestoreTurnPoints();
		}
	}
}

void MapManager::NextLevel(int numberOfRooms)
{
	// Generate map
	MapGenerator mapGenerator(&_map);
	mapGenerator.GenerateMap(numberOfRooms);
	// Generate objects on map
	GenerateObjectsOnMap();
}

void MapManager::GameOver()
{
	// Make player lose everything he got
	_playerCharacter->RemoveAllItems();
	// Remove all objects from map
	RemoveAllObjectsFromMap();
	// ...
	// MOVE TO SCORE MENU
}

MapManager::MapManager(WindowManager* windowManager, InterfaceManager* interfaceManager) : _playerCharacter(nullptr),
	_levelsPassed(0), _map(MAP_DEFAULT_SIZE_X, MAP_DEFAULT_SIZEE_Y), _turnState(CharacterType::PlayerType),
	_windowManager(windowManager), _interfaceManager(interfaceManager)
{
	// "Subscribe" to window manager, to get messages about events
	_windowManager->AddEventListener(this);
	// Push all elements in window manager
	for (int i = 0; i < _map.GetMapHeight(); i++)
	{
		for (int j = 0; j < _map.GetMapWidth(); j++)
		{
			_windowManager->AddObject(Layer::BackgroundLayer, &_map.GetMap2D()[i][j]);
		}
	}
	// Initialize ladder
	_ladder = new Ladder(&_map, nullptr);
	// Add it to the window
	_windowManager->AddObject(Layer::ForegroundLayer,_ladder);
	// Subscribe on ladder event
	_ladder->AddEventListener(this);
}

void MapManager::RemoveAllObjectsFromMap()
{
	// Remove player from map
 	_playerCharacter->RemoveFromMapCell();
	// Remove ladder from map
	_ladder->RemoveFromMapCell();
	// Remove all enemies
	for (std::list<EnemyCharacter*>::iterator it = _enemyCharacterList.begin();
		it != _enemyCharacterList.end(); it++)
	{
		// Remove enemy from window manager
		_windowManager->RemoveObject(*it);
		// Remove enemy from map cell
		(**it).RemoveFromMapCell();
		// Delete object
		delete* it;
	}
	_enemyCharacterList.erase(_enemyCharacterList.begin(), _enemyCharacterList.end());
	// Make map visible (it is used to define items, which were not picked up by any character)
	_map.SetRectangleVisibility(Point2DInt(0, 0), 
		Point2DInt(_map.GetMapWidth() - 1, _map.GetMapHeight() - 1), true);
	// Delete all visible items
	std::list<Item*>::iterator it = _itemList.begin();
	while(it != _itemList.end())
	{
		// Remove item from map cell
		(**it).RemoveFromMapCell();
		// Remove object, if it is visible (in this situation, it means that
		// it was left on the map) or is dead
		if ((**it).IsObjectVisible() || (**it).IsObjectDead())
		{
			// Remove enemy from window manager
			_windowManager->RemoveObject(*it);
			// Delete object
			delete* it;
			// Remove this object from list
			_itemList.remove(*it);
			// Go back to the beginning
			it = _itemList.begin();
			continue;
		}
		it++;
	}
	// Make map invisible
	_map.SetRectangleVisibility(Point2DInt(0, 0),
		Point2DInt(_map.GetMapWidth() - 1, _map.GetMapHeight() - 1), false);
}

void MapManager::GenerateObjectsOnMap()
{
	// Removes all objects from map
	RemoveAllObjectsFromMap();
	// Place player in the first room
	RandomRoomPositionSet(*_map.GetRooms().begin(), _playerCharacter);
	// Place ladder in the last room
	RandomRoomPositionSet(_map.GetRooms().back(), _ladder);
	// Randomly generate number of items and enemies on the level
	int enemyNumber = rand() % _map.GetRooms().size() + 1;
	int itemNumber = rand() % _map.GetRooms().size() + 1;
	// Randomly generate different enemies and place them on the level
	for (int i = 0; i < enemyNumber; i++)
	{
		int randEnemy = rand() % 3;
		EnemyCharacter* enemy = nullptr;
		switch (randEnemy)
		{
			// Light enemy
		default:
		case 0:
			enemy = new LightEnemyCharacter(_playerCharacter, &_map, nullptr);
			break;
			// Medium enemy
		case 1:
			enemy = new MediumEnemyCharacter(_playerCharacter, &_map, nullptr);
			break;
			// Large enemy
		case 2:
			enemy = new LargeEnemyCharacter(_playerCharacter, &_map, nullptr);
			break;
		}
		// Try to place enemy in random room
		if (!RandomRoomPositionSet(_map.GetRooms()[rand() % _map.GetRooms().size()], enemy))
		{
			// If there are no free position for this object - delete it
			delete enemy;
			continue;
		}
		// Set enemy turn points to zero
		enemy->SetRecentTurnPoints(0);
		// Add enemy to the list
		_enemyCharacterList.push_back(enemy);
		// Add enemy to the window
		_windowManager->AddObject(Layer::ForegroundLayer, enemy);
	}
	// Randomly generate different items and place them on the level
	for (int i = 0; i < itemNumber; i++)
	{
		// Random item 
		int randItem = rand() % 5;
		// Random weapon (in case of this item generated)
		int randWeapon;
		// Item
		Item* item = nullptr;
		switch (randItem)
		{
			// Gold
		default:
		case 0:
			item = new Gold(&_map, nullptr, rand() % 10 + 1);
			break;
			// Armor
		case 1:
			item = new Armor(&_map, nullptr, rand() % 10 + 1);
			break;
			// Weapon
		case 2:
			randWeapon = rand() % 3;
			switch (randWeapon)
			{
			default:
				// Dagger
			case 0:
				item = new WeaponDagger(&_map, nullptr);
				break;
				// Sword
			case 1:
				item = new WeaponSword(&_map, nullptr);
				break;
				// Mace
			case 2:
				item = new WeaponMace(&_map, nullptr);
				break;
			}
			break;
			// Heal potion
		case 3:
			item = new HealPotion(&_map, nullptr, rand() % 10 + 1);
			break;
			// Turn potion
		case 4:
			item = new TurnPotion(&_map, nullptr, rand() % 5 + 1);
			break;
		}
		// Try to place item in random room
		if (!RandomRoomPositionSet(_map.GetRooms()[rand() % _map.GetRooms().size()], item))
		{
			// If there are no free position for this object - delete it
			delete item;
			continue;
		}
		// Add item to the list
		 _itemList.push_back(item);
		// Add item to the window 
		 _windowManager->AddObject(Layer::ForegroundLayer, item);
	}
	// Set player turn
	_turnState = CharacterType::PlayerType;
	// Update player turn points
	_playerCharacter->RestoreTurnPoints();
}

bool MapManager::RandomRoomPositionSet(Room& room, InteractiveObject* interactiveObject)
{
	// Loop through all position in the room and form list of free positions
	std::list<Point2DInt> freePositionsList;
	// We make +1 offset, so we won't check walls, which are always uncreachable position
	for (int y = room.GetLeftTopPosition().y + 1; y < room.GetRightBottomPosition().y; y++)
	{
		for (int x = room.GetLeftTopPosition().x + 1; x < room.GetRightBottomPosition().x; x++)
		{
			if (_map.GetMap2D()[y][x].GetCellState() == CellState::Free)
			{
				freePositionsList.push_back(_map.GetMap2D()[y][x].GetArrayPosition());
			}
		}
	}
	// If there are no free positions - return false
	if (freePositionsList.size() <= 0) { return false; }
	// Randomly choose position and try to place our interactive object there
	return interactiveObject->SetCellObjectPosition
	(*std::next(freePositionsList.begin(),rand() % freePositionsList.size()));
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
		switch (windowEvent.key.code)
		{
		case sf::Keyboard::Up:
			// Move up
			_playerCharacter->MoveCellObject(PlacementDirection::Top, targetMapCell);
			break;
			// Move down
		case sf::Keyboard::Down:
			_playerCharacter->MoveCellObject(PlacementDirection::Bottom, targetMapCell);
			break;
			// Move left
		case 	sf::Keyboard::Left:
			_playerCharacter->MoveCellObject(PlacementDirection::Left, targetMapCell);
			break;
			// Move right
		case sf::Keyboard::Right:
			_playerCharacter->MoveCellObject(PlacementDirection::Right, targetMapCell);
			break;
			// Swaps weapons
		case sf::Keyboard::S:
			_playerCharacter->SwapWeapons();
			break;
			// Use heal potion
		case sf::Keyboard::H:
			_playerCharacter->UseHealPotion();
			break;
			// Use turn potion
		case sf::Keyboard::T:
			_playerCharacter->UseTurnPotion();
			break;
			// Pass the turn
		case sf::Keyboard::Space:
			_playerCharacter->UseTurnPoint();
			break;
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
		NextLevel(10);
		// Increment level counter
		_levelsPassed++;
	}
}

void MapManager::StartGameplay()
{
	// Update level counter
	_levelsPassed = 0;
	// Delete player
	delete _playerCharacter;
	// Initialize player
	_playerCharacter = new PlayerCharacter(&_map, nullptr, "Player", 5, 5, 10, 5, 5);
	// Add player to window
	_windowManager->AddObject(Layer::ForegroundLayer, _playerCharacter);
	// Generate level
	NextLevel(5);
}