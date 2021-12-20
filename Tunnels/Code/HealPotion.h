#pragma once

#include "PotionHolder.h"

class HealPotion : public PotionHolder
{
public:
	HealPotion(Map* map, MapCell* mapCell, int points)
		: PotionHolder(map, mapCell, "Heal Potion", points)
	{
		// Load texture
		SetTexture("Textures\\HealPotion.png");
	}
	// Gets heal potion
	PotionHolder* GetHealPotion() override { return this; }
};