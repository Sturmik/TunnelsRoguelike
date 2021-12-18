#include "PlayerCharacter.h"

void PlayerCharacter::OnPositionChange() 
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
}

void PlayerCharacter::CharacterInteraction(BaseCharacter* charObj)
{
	// If character object is an enemy - fight it
	if (charObj->GetCharacterType() == CharacterType::EnemyType)
	{
		Fight(charObj);
	}
}

void PlayerCharacter::TriggerInteraction(Trigger* triggerObj)
{
	// Invoke trigger
	triggerObj->Invoke();
}