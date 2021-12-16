#pragma once

#include "InteractiveObject.h"

class WeaponHolder;
class PotionHolder;

// Passive objects in the game.
// They can be picked up by characters objects
// and used by them.
// By overriding given function, we can define new items
class Item : public InteractiveObject
{
protected:
	Item(Map* map, MapCell* mapCell,
		std::string itemName) : InteractiveObject(map, mapCell, itemName)
	{ }
public:
	// Removes item from cell and makes it invisible
	void RemoveFromMapCell() override 
	{
		_mapCell->SetCellState(CellState::Free);
		_mapCell->SetGameObject(nullptr);
		SetObjectVisibility(false);
	}
	// Get object type
	ObjectType GetObjectType() const override { return ObjectType::ItemObject; }
	// Get gold
	virtual int GetGold() { return 0; }
	// Get armor
	virtual int GetArmor() { return 0; }
	// Get heal potion
	virtual PotionHolder* GetHealPotion() { return nullptr; }
	// Get turn potion
	virtual PotionHolder* GetTurnPotion() { return nullptr; }
	// Get weapon holder
	virtual WeaponHolder* GetWeaponHolder() { return nullptr; }
};