#pragma once

#include "BaseCharacter.h"

// Player character, which is controlled by user
class PlayerCharacter : public BaseCharacter
{
protected:
	// Character interaction
	virtual void CharacterInteraction(BaseCharacter* charObj) override
	{
		// If character object is enemy - fight it
		if (charObj->GetCharacterType() == CharacterType::EnemyType)
		{
			Fight(charObj);
		}
	}

	// Trigger interaction
	virtual void TriggerInteraction(Trigger* triggerObj)
	{
		// Check, if trigger is a ladder
		Ladder* ladder = dynamic_cast<Ladder*>(triggerObj);
		// If it is ladder invoke it's method
		if (ladder != nullptr)
		{
			ladder->Invoke();
		}
	}

public:
	PlayerCharacter(Map* map,
		MapCell* mapCell, std::string name, int maxWalkPoints, int health,
		int armor, int strength, int goldCount) : BaseCharacter(map,
			mapCell,name, maxWalkPoints,
			health, armor, strength, goldCount) 
	{	
		// Load texture
		SetTexture("Textures\\PlayerCharacter.png");
	}

	// Get character type
	virtual CharacterType GetCharacterType() const override
	{
		return CharacterType::PlayerType;
	}
};