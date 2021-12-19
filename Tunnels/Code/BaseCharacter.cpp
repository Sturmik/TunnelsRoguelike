#include "BaseCharacter.h"

BaseCharacter::~BaseCharacter()
{
	std::cout << _objectName << " is defeated!" << std::endl;
	// Delete all weapons
	while (!_weaponQueue.empty())
	{
		_weaponQueue.front()->SetObjectDeathState(true);
		_weaponQueue.pop();
	}
	// Delete all heal potions
	while (!_healPotionStack.empty())
	{
		_healPotionStack.top()->SetObjectDeathState(true);
		_healPotionStack.pop();
	}
	// Delete all turn potions
	while (!_turnPotionsStack.empty())
	{
		_turnPotionsStack.top()->SetObjectDeathState(true);
		_turnPotionsStack.pop();
	}
}

void BaseCharacter::ItemInteraction(Item* itemObj)
{
	// Get item out of its map cell
	itemObj->RemoveFromMapCell();
	// Log about this
	std::cout  << _objectName << " picked up a " << itemObj->GetObjectName() << std::endl;
	// Get armor
	_armor += itemObj->GetArmor();
	// Get gold
	_goldCount += itemObj->GetGold();
	// Get heal potion
	if (itemObj->GetHealPotion() != 0)
	{
		_healPotionStack.push(itemObj->GetHealPotion());
	}
	// Get walk potion
	if (itemObj->GetTurnPotion() != 0)
	{
		_turnPotionsStack.push(itemObj->GetTurnPotion());
	}
	// Get weapon holder
	if (itemObj->GetWeaponHolder() != 0)
	{
		_weaponQueue.push(itemObj->GetWeaponHolder());
	}
}

void BaseCharacter::TakeDamage(int damage)
{
	std::cout << _objectName << " takes damage " << damage << std::endl;
	// If damage is larger than armor, deal it to the health
	// but decrease it by armor level
	if (damage > _armor)
	{
		_recentHealth -= damage - _armor;
	}
	// Decrease armor
	_armor -= damage;
	// If armor is lesser than zero, set it to zero
	if (_armor <= 0)
	{
		_armor = 0;
	}
	// If recent health is lower than zero - kill gameobject
	if (_recentHealth <= 0)
	{
		_recentHealth = _recentTurnPoints=  0;
		SetObjectDeathState(true);
		RemoveFromMapCell();
	}

	std::cout << _objectName << " - hp left: " << _recentHealth << std::endl;
	std::cout << _objectName << " - armor left " << _armor << std::endl;
}

void BaseCharacter::GetLooted(BaseCharacter* opponent)
{
	std::cout << _objectName << " gets looted by " << opponent->GetObjectName() << std::endl;
	// Loot some gold
	opponent->_goldCount += _goldCount / 4;
	_goldCount = 0;
	// Loot armor
	opponent->_armor += _armor / 4;
	_armor = 0;
	// Loot recent weapon
	if (_weaponQueue.size() > 0)
	{
		opponent->_weaponQueue.push(_weaponQueue.front());
		_weaponQueue.pop();
	}
	// Loot potions
	if (_healPotionStack.size() > 0)
	{
		opponent->_healPotionStack.push(_healPotionStack.top());
		_healPotionStack.pop();
	}
	if (_turnPotionsStack.size() > 0)
	{
		opponent->_turnPotionsStack.push(_turnPotionsStack.top());
		_turnPotionsStack.pop();
	}
}

int BaseCharacter::UseRecentWeapon()
{
	// If, we don't have a weapon - return zero
	if (_weaponQueue.size() == 0) { return 0; }
	// Go through all weapons, and delete weapons, which are broken
	while (_weaponQueue.front()->GetWeapon().GetDurability() == 0)
	{
		// Set this item to dead state
		_weaponQueue.front()->SetObjectDeathState(true);
		// Remove it from front
		_weaponQueue.pop();
		// Check again, if we don't have a weapon - return zero
		if (_weaponQueue.size() == 0) { return 0; }
	}
	// Else, use weapon
	std::cout << _objectName << " used the " << _weaponQueue.front()->GetObjectName() 
		<< ", power =  " << _weaponQueue.front()->GetWeapon().GetPower() << ", durability = " << 
		_weaponQueue.front()->GetWeapon().GetDurability() << std::endl;
	return _weaponQueue.front()->GetWeapon().UseWeapon();
}

bool BaseCharacter::UseRecentHealPotion()
{
	// If we don't have a potion - return false
	if (_healPotionStack.size() == 0) { return false; }
	// Go through all potions, and delete potions, which are consumed
	while (_healPotionStack.top()->GetPotion().CheckPoints() == 0)
	{
		// Set this item to dead state
		_healPotionStack.top()->SetObjectDeathState(true);
		// Remove it from front
		_healPotionStack.pop();
		// Check again, if we don't have a potion - return false
		if (_healPotionStack.size() == 0) { return false; }
	}
	// Use turn point, if any left
	if (!UseTurnPoint()) { return false; };
	// Consume potion
	std::cout << _objectName << " consumed the " << _healPotionStack.top()->GetObjectName() 
		<< " - " << _healPotionStack.top()->GetPotion().CheckPoints() << std::endl;
	_recentHealth += _healPotionStack.top()->GetPotion().ConsumePoints();
	if (_recentHealth > _maxHealth) { _recentHealth = _maxHealth; }
	return true;
}

bool BaseCharacter::UseRecentTurnPotion()
{
	// If we don't have a potion - return false
	if (_turnPotionsStack.size() == 0) { return false; }
	// Go through all potions, and delete potions, which are consumed
	while (_turnPotionsStack.top()->GetPotion().CheckPoints() == 0)
	{
		// Set this item to dead state
		_turnPotionsStack.top()->SetObjectDeathState(true);
		// Remove it from front
		_turnPotionsStack.pop();
		// Check again, if we don't have a potion - return false
		if (_turnPotionsStack.size() == 0) { return false; }
	}
	// Use turn point, if any left
	if (!UseTurnPoint()) { return false; };
	// Consume potion
	std::cout << _objectName << " consumed the " << _turnPotionsStack.top()->GetObjectName()
		<< " - " << _turnPotionsStack.top()->GetPotion().CheckPoints() << std::endl;
	_recentTurnPoints += _turnPotionsStack.top()->GetPotion().ConsumePoints();
	if (_recentTurnPoints > _maxTurnPoints) { _recentTurnPoints = _maxTurnPoints; }
	return true;
}

void BaseCharacter::Interact(InteractiveObject* interactObj)
{
	// If pointer is nullptr, 
	// or if it is the same object as interacts, 
	// or object is dead - return
	if (interactObj == nullptr 
		|| interactObj == this 
		|| IsObjectDead()) { return; }
	// Check, if there are no turn points left
	if (!UseTurnPoint()) { return; }
	// Define type of behaviour with an object
	switch (interactObj->GetObjectType())
	{
		// Character interaction
	case ObjectType::CharacterObject:
		CharacterInteraction(dynamic_cast<BaseCharacter*>(interactObj));
		break;
		// Item interaction
	case ObjectType::ItemObject:
		ItemInteraction(dynamic_cast<Item*>(interactObj));
		break;
		// Trigger interaction
	case ObjectType::TriggerObject:
		TriggerInteraction(dynamic_cast<Trigger*>(interactObj));
		break;
	default:
		break;
	}
}

void BaseCharacter::Fight(BaseCharacter* opponentCharacter)
{
	std::cout << _objectName << " fights " << opponentCharacter->GetObjectName() << std::endl;
	// Calculate hit force
	int hitForce = rand() % (_strength + UseRecentWeapon()) + 1;
	// Try damage this object
	opponentCharacter->TakeDamage(hitForce);
	// If opponent was defeated, loot him
	if (opponentCharacter->GetRecentHealth() <= 0)
	{
		opponentCharacter->GetLooted(this);
	}
}