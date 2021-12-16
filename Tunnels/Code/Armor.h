#pragma once

#include "Item.h"

// Armor class is used for avoding damage
class Armor : public Item
{
private:
	int _armor;
public:
	Armor(Map* map, MapCell* mapCell, int armor)
		: Item(map, mapCell, "Armor"), _armor(armor) 
	{
		// Load texture
		SetTexture("Textures\\Armor.png");
	}

	int GetArmor() override { return  _armor; }
};