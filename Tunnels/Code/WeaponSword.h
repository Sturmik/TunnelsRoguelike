#pragma once

#include "WeaponHolder.h"

// Sword weapon
// - Medium durability
// - Medium damage
class WeaponSword : public WeaponHolder
{
public:
	WeaponSword(Map* map, MapCell* mapCell)
		: WeaponHolder(map, mapCell, "Sword", Weapon(5, 5))
	{
		// Load texture
		SetTexture("Textures\\WeaponSword.png");
	}
};