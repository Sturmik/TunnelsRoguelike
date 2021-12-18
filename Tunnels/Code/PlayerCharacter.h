#pragma once

#include "BaseCharacter.h"

// Player character, which is controlled by user
// If player reaches new room or cell, he sets its visibility state to true
class PlayerCharacter : public BaseCharacter
{
protected:
	// Override on position change method to update visibility state of map cells
	void OnPositionChange() override;
	// Character interaction
	void CharacterInteraction(BaseCharacter* charObj) override;
	// Trigger interaction
	void TriggerInteraction(Trigger* triggerObj) override;
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
	CharacterType GetCharacterType() const override
	{
		return CharacterType::PlayerType;
	}
};