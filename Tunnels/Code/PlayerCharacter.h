#pragma once

#include "BaseCharacter.h"

// Player character, which is controlled by user
// If player reaches new room or cell, he sets its visibility state to true
class PlayerCharacter : public BaseCharacter
{
protected:
	// Override on position change method to update visibility state of map cells
	void OnPositionChange() override
	{
		// Player is always visible
		_mapCell->SetMapCellVisibility(true);
		// Make cells around visibile
		_map->SetRectangleVisibility(Point2DInt(_mapCell->GetArrayPosition().x - 1, _mapCell->GetArrayPosition().y - 1),
			Point2DInt(_mapCell->GetArrayPosition().x + 1, _mapCell->GetArrayPosition().y + 1), true);
		// Check, if player is inside of the room
		Room* room = _map->GetRoomByPosition(_mapCell->GetArrayPosition());
		// If so, make it visible
		_map->SetRoomVisibility(room, true);
	};

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