#pragma once

#include "Ladder.h"
#include "Gold.h"
#include "Armor.h"
#include "WeaponHolder.h"
#include "WeaponHolder.h"
#include "PotionHolder.h"

#include <queue>
#include <stack>
#include <string>
#include <iostream>

// Character types
enum CharacterType
{
	// Type, which is controlled by user
	PlayerType,
	// Type, which is controlled by computer
	EnemyType
};

class BaseCharacter : public InteractiveObject
{
protected:
	// Max turn points per one turn
	int _maxTurnPoints;
	// Recent turn points during recent turn
	int _recentTurnPoints;

	// Character maximum amount of health
	int _maxHealth;
	// Character health, if reaches zero - character dies
	int _recentHealth;
	// Character armor, for damage avoiding
	int _armor;
	// Character natural strength, for dealing damage
	// By natural, we mean damage without any additions, such as
	// weapons or potions
	int _strength;
	// Amount of gold, which character carries
	int _goldCount;

	// Weapons, which character carries
	std::queue<WeaponHolder*> _weaponQueue;
	// Heal potions, which character carries
	std::stack<PotionHolder*> _healPotionStack;
	// Walk potions, which character carries
	std::stack<PotionHolder*> _turnPotionsStack;

	BaseCharacter(Map* map,
		MapCell* mapCell, std::string name,
		int maxWalkPoints, int maxHealth,
		int armor, int strength, int goldCount)
		: InteractiveObject(map, mapCell,
			name), _maxTurnPoints(maxWalkPoints), _recentTurnPoints(0),
		_maxHealth(maxHealth), _recentHealth(maxHealth),
		_armor(armor), _strength(strength), _goldCount(goldCount) {}

	~BaseCharacter();

	// Override before change position method
	// So, now it will allow object to move, only when it has turn points
	bool BeforePositionChange() override {	return UseTurnPoint() && !IsObjectDead();}

	// Derived classes must implement these functions to define interaction behaviour

	// Interaction with characters
	virtual void CharacterInteraction(BaseCharacter* charObj) = 0;
	// Interaction with items
	virtual void ItemInteraction(Item* itemObj);
	// Interaction with trigger
	virtual void TriggerInteraction(Trigger* triggerObj) = 0;

	// Take damage 
	void TakeDamage(int damage);

	// Get looted by another character
	void GetLooted(BaseCharacter* opponent);
public:
	// Get object type
	ObjectType GetObjectType()const  override { return ObjectType::CharacterObject; }

	// Get character type
	virtual CharacterType GetCharacterType() const = 0;

	// Try to use turn point, if there are ones
	// Return true, if point was used
	// Return false, if there are no points left
	bool UseTurnPoint() 
	{	
		std::cout << _objectName << " has " << _recentTurnPoints - 1 << " turn points left" << std::endl;
		if (_recentTurnPoints > 0) { _recentTurnPoints--; return true; }
		return false;
	}

	// Restores turn points to maximum available
	void RestoreTurnPoints() { _recentTurnPoints = _maxTurnPoints; }

	// Get recent turn points
	int GetRecentTurnPoints() const { return _recentTurnPoints; }

	// Get recent health
	int GetRecentHealth() const { return _recentHealth; }

	// Get armor
	int GetArmor() const { return _armor; }

	// Get strength
	int GetStrength() const { return _strength; }

	// Get gold count
	int GetGoldCount() const { return _goldCount; }

	// Uses recent weapon (front in queue)
	int UseRecentWeapon();

	// Uses recent heal potion
	bool UseRecentHealPotion();

	// Uses recent heal potion
	bool UseRecentTurnPotion();

	// Interact with object
	void Interact(InteractiveObject* interactObj);

	// Fight procedure (Recent object attacks another)
	void Fight(BaseCharacter* opponentCharacter);
};