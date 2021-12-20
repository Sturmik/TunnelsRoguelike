#pragma once

#include "WeaponHolder.h"

// Mace weapon
// - High durability
// - Low damage
class WeaponMace : public WeaponHolder
{
public:
	WeaponMace(Map* map, MapCell* mapCell)
		: WeaponHolder(map, mapCell, "Mace", Weapon(5, 5))
	{
		// Load texture
		SetTexture("Textures\\WeaponMace.png");
	}
};