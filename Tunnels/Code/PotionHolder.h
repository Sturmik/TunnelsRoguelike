#pragma once

#include "Item.h"

// Potion holder class contains poition inside of it
class PotionHolder : public Item
{
public:

	// Potion class is used for defining different kinds of potions
	class Potion
	{
	private:
		// Points to increase specific characteristic
		int _points;
	public:
		Potion(int points) : _points(points) {}

		// Checks potion points, without consuming it
		int CheckPoints()
		{
			return _points;
		}

		// Consume potion and empty it
		int ConsumePoints()
		{
			int returnValue = _points;
			_points = 0;
			return returnValue;
		}
	};

	// Get points inside of potion holder
	Potion& GetPotion() { return _potion; }
protected:
	Potion _potion;

	PotionHolder(Map* map, MapCell* mapCell, std::string name, int points)
		: Item(map, mapCell, name), _potion(points) {}
};

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