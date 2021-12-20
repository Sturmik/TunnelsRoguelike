#pragma once

// Include manager
#include "InterfaceManager.h"
// Include map generator
#include "MapGenerator.h"
// Include all game objects
#include "Ladder.h"
#include "PlayerCharacter.h"
#include "LightEnemyCharacter.h"
#include "MediumEnemyCharacter.h"
#include "LargeEnemyCharacter.h"
#include "Gold.h"
#include "Armor.h"
#include "WeaponDagger.h"
#include "WeaponSword.h"
#include "WeaponMace.h"
#include "HealPotion.h"
#include "TurnPotion.h"

// Map manager controls almost everything that is related to gameplay
// - Map/Object generation
// - Object moving
// - Player object control
class MapManager : WindowEventListener, MethodEventListener
{
private:
	// Default constant size of the map
	const int MAP_DEFAULT_SIZE_X = 120;
	const int MAP_DEFAULT_SIZEE_Y = 60;

	// Map with game objects
	Map _map;

	// Counts how many level player has passed
	int _levelsPassed;
	// Defines turn cycle
	CharacterType _turnState;

	// Player and ladder objects are created once

	// Player object
	PlayerCharacter* _playerCharacter;
	// Ladder object
	Ladder* _ladder;

	// Enemies and items are created every new level

	// Enemy character list
	std::list<EnemyCharacter*> _enemyCharacterList;
	// Item list
	std::list<Item*> _itemList;

	// Window manager
	WindowManager* _windowManager;
	// Interface manager pointer for interface updates
	InterfaceManager* _interfaceManager;

	// Removes all objects from map
	void RemoveAllObjectsFromMap();
	// Places player and ladder in new positions
	// Removes all previous objects on map
	// and generates new ones 
	void GenerateObjectsOnMap();
	// Randomly sets position in room for given game object
	bool RandomRoomPositionSet(Room& room, InteractiveObject* interactiveObject);

	// Window event processing
	virtual void WindowEventProcess(sf::RenderWindow& window, sf::Event windowEvent) override;
	// Event processing
	virtual void EventProcess(GameObject* gameObjectEvent) override;

	// Generates next map and updates objects
	void NextLevel(int numberOfRooms);
	// Game over happens after player dies
	void GameOver();
public:
	MapManager(WindowManager* windowManager, InterfaceManager* interfaceManager);

	~MapManager();

	// Updates state of map manager
	void Update();

	// Starts gameplay
	void StartGameplay();
};