#pragma once

#include "MapGenerator.h"
#include "InterfaceManager.h"

#include "PlayerCharacter.h"
#include "Gold.h"
#include "Armor.h"
#include "WeaponHolder.h"
#include "PotionHolder.h"

// Map manager controls almost everything that is related to gameplay
// - Map/Object generation
// - Object moving
// - Player object control
class MapManager : WindowEventListener
{
private:
	bool inputTop = false;
	bool inputBottom = false;

	// Default constant size of the map
	const int MAP_DEFAULT_SIZE_X = 120;
	const int MAP_DEFAULT_SIZEE_Y = 60;

	// Map with game objects
	Map _map;

	// Player object
	PlayerCharacter* _playerCharacter;

	// TEST
	Armor* _armorItem;
	WeaponHolder* _weaponHolder;
	PotionHolder* _healPotionHolder;
	PotionHolder* _turnPotionHolder;

	// Window manager
	WindowManager* _windowManager;
	// Interface manager pointer for interface updates
	InterfaceManager* _interfaceManager;
public:
	MapManager(WindowManager* windowManager, InterfaceManager* interfaceManager) : 
		_map(MAP_DEFAULT_SIZE_X, MAP_DEFAULT_SIZEE_Y), 
		_windowManager(windowManager), _interfaceManager(interfaceManager)
	{
		// "Subscribe" to window manager, to get messages about events
		_windowManager->AddEventListener(this);
		// Push all elements in window manager
		for (int i = 0; i < _map.GetMapHeight(); i++)
		{
			for (int j = 0; j < _map.GetMapWidth(); j++)
			{
				_windowManager->AddObject(Layer::BackLayer, &_map.GetMap2D()[i][j]);
			}
		}
		// Initialize objects TEST
		_playerCharacter = new PlayerCharacter(&_map,nullptr, "Player", 5, 5, 10, 2, 5);
		_armorItem = new Armor(&_map, nullptr, 5);
		_weaponHolder = new WeaponHolder(&_map, nullptr, "PainBringer", WeaponHolder::Weapon(50, 2));
		_healPotionHolder = new HealPotion(&_map, nullptr, 5);
		_turnPotionHolder = new TurnPotion(&_map, nullptr, 10);

		_windowManager->AddObject(Layer::FrontLayer, _playerCharacter);
		_windowManager->AddObject(Layer::FrontLayer, _armorItem);
		_windowManager->AddObject(Layer::FrontLayer, _weaponHolder);
		_windowManager->AddObject(Layer::FrontLayer, _healPotionHolder);
		_windowManager->AddObject(Layer::FrontLayer, _turnPotionHolder);
	}

	~MapManager() 
	{
		// delete _playerCharacter;
		_windowManager->RemoveListener(this); 
	}

	// Updates state of the program
	void Update();
	// Generates map with all objects on it
	void GenerateMap(int numberOfRooms);
	// Get map
	Map& GetMap() { return _map; }

	// Window event processing
	virtual void WindowEventProcess(sf::RenderWindow& window, sf::Event windowEvent) override;
};