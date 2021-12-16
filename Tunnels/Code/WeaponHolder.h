#pragma once

#include "Item.h"

// Weapon holder class contains weapon inside of it
class WeaponHolder : public Item
{
public:
	// Weapon class is used for fights 
	class Weapon
	{
	private:
		// Power of weapon
		int _power;
		// Durability, defines number of times it can be used
		int _durability;
	public:
		int GetPower() const { return _power; }
		int GetDurability() const { return _durability; }

		// Uses weapon, returns power and decrease durability by one
		int UseWeapon()
		{
			// If weapon is broken, return zero power
			if (_durability == 0) { return 0; }
			_durability--;
			return _power;
		}

		Weapon(int power, int durability) : _power(power), _durability(durability) {}
	};

	WeaponHolder(Map* map, MapCell* mapCell, std::string name, Weapon weapon)
		: Item(map, mapCell, name), _weapon(weapon) 
	{
		// Load texture
		SetTexture("Textures\\Weapon.png");
	}

	// Gets weapon
	Weapon& GetWeapon() { return _weapon; }
	WeaponHolder* GetWeaponHolder() override { return this; }
private:
	Weapon _weapon;
};