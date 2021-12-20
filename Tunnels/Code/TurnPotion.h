#pragma once

#include "PotionHolder.h"

class TurnPotion : public PotionHolder
{
public:
	TurnPotion(Map* map, MapCell* mapCell, int points)
		: PotionHolder(map, mapCell, "Turn Potion", points)
	{
		// Load texture
		SetTexture("Textures\\TurnPotion.png");
	}
	// Gets walk potion
	PotionHolder* GetTurnPotion() override { return this; }
};