#pragma once

#include "Item.h"

// Gold class is used for score calculation
class Gold : public Item
{
private:
	// Value of this specific gold object
	int _gold;
public:
	Gold(Map* map, MapCell* mapCell, int gold)
		: Item(map, mapCell, "Gold"), _gold(gold) { }

	int GetGold() override { return _gold; }
};
