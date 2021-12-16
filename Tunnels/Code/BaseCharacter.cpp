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
	// Mark object as "Dead", because now it will only appear in our inventory
	itemObj->SetObjectDeathState(true);
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
	// If damage is lesser than amor,
	// don't take damage and decrease armor
	if (damage <= _armor)
	{
		_armor -= _armor / 4;
	}
	// Else, deal damage to the health
	else
	{
		_recentHealth -= damage;
	}
	// If armor is lesser than zero, set it to zero
	if (_armor < 0)
	{
		_armor = 0;
	}
	// If recent health is lower than zero - kill gameobject
	if (_recentHealth < 0)
	{
		_recentHealth = 0;
		SetObjectDeathState(true);
	}
}

void BaseCharacter::GetLooted(BaseCharacter* opponent)
{
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
		// Delete weapon
		delete _weaponQueue.front();
		// Remove it from front
		_weaponQueue.pop();
	}
	// Check again, if we don't have a weapon - return zero
	if (_weaponQueue.size() == 0) { return 0; }
	// Else, use weapon
	else { return _weaponQueue.front()->GetWeapon().UseWeapon(); }
}

bool BaseCharacter::UseRecentHealPotion()
{
	// If we don't have a potion - return false
	if (_healPotionStack.size() == 0) { return false; }
	// Decrease walk points
	_recentTurnPoints--;
	// Go through all potions, and delete potions, which are consumed
	while (_healPotionStack.top()->GetPotion().CheckPoints() == 0)
	{
		// Delete potion
		delete _healPotionStack.top();
		// Remove it from front
		_healPotionStack.pop();
	}
	// Check again, if we don't have a potion - return false
	if (_healPotionStack.size() == 0) { return false; }
	// Else consume potion
	else
	{
		_recentHealth += _healPotionStack.top()->GetPotion().ConsumePoints();
		if (_recentHealth > _maxHealth) { _recentHealth = _maxHealth; }
		return true;
	}
}

bool BaseCharacter::UseRecentTurnPotion()
{
	// If we don't have a potion - return false
	if (_turnPotionsStack.size() == 0) { return false; }
	// Go through all potions, and delete potions, which are consumed
	while (_turnPotionsStack.top()->GetPotion().CheckPoints() == 0)
	{
		// Delete potion
		delete _turnPotionsStack.top();
		// Remove it from front
		_turnPotionsStack.pop();
	}
	// Check again, if we don't have a potion - return false
	if (_turnPotionsStack.size() == 0) { return false; }
	// Else consume potion
	else
	{
		_recentTurnPoints += _turnPotionsStack.top()->GetPotion().ConsumePoints();
		if (_recentTurnPoints > _maxTurnPoints) { _recentTurnPoints = _maxTurnPoints; }
		return true;
	}
}

void BaseCharacter::Interact(InteractiveObject* interactObj)
{
	// Decrease turn points
	_recentTurnPoints--;
	if (interactObj == nullptr) { return; }
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
	case ObjectType::TriggerObject:
		TriggerInteraction(dynamic_cast<Trigger*>(interactObj));
		break;
	default:
		break;
	}
}

void BaseCharacter::Fight(BaseCharacter* opponentCharacter)
{
	// Decrease turn points
	_recentTurnPoints--;
	// Calculate hit force
	int hitForce = rand() % (_strength + UseRecentWeapon());
	// Try damage this object
	opponentCharacter->TakeDamage(hitForce);
	// If opponent was defeated, loot him
	if (opponentCharacter->GetRecentHealth() <= 0)
	{
		opponentCharacter->GetLooted(this);
	}
}