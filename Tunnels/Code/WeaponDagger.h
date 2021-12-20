#pragma once

#include "WeaponHolder.h"

// Dagger weapon
// - Low durability
// - High damage
class WeaponDagger : public WeaponHolder
{
public:
	WeaponDagger(Map* map, MapCell* mapCell)
		: WeaponHolder(map, mapCell, "Dagger", Weapon(10, 2))
	{
		// Load texture
		SetTexture("Textures\\WeaponDagger.png");
	}
};