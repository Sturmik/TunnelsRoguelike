#include "EnemyCharacter.h"

void EnemyCharacter::CharacterInteraction(BaseCharacter* charObj)
{
	// If character object is player - fight it
	if (charObj->GetCharacterType() == CharacterType::PlayerType)
	{
		Fight(charObj);
	}
}

void EnemyCharacter::TriggerInteraction(Trigger* triggerObj)
{
	// Enemy doesn't interact with trigger objects
}

void EnemyCharacter::Update()
{
	// If player or enemy doesn't have assigned map cell, or object is dead - return
	if (_mapCell == nullptr || _playerCharacter->GetMapCell() == nullptr || IsObjectDead()) { return; }
	// Caclculate distance to player
	float distanceToPlayer = Point2DInt(getPosition().x, getPosition().y).
		Distance(Point2DInt(_playerCharacter->getPosition().x, _playerCharacter->getPosition().y));
	// If player is nearby and there are no turn points left - try to use turn potion
	if (distanceToPlayer < ENEMY_VIEW_RADIUS && GetRecentTurnPoints() <= 0) { UseRecentTurnPotion(); return; }
	// If there are no more points left - return
	if (GetRecentTurnPoints() <= 0) { return; }
	// If health if lesser than half of full - heal
	if (_recentHealth < _recentHealth / 2) { UseRecentHealPotion(); return; }
	// If there is player nearby - chase and fight him
	// If player is inside of the enemy view radius - form way to him
	if (distanceToPlayer < ENEMY_VIEW_RADIUS)
	{
		_pathToPlayer = _pathFindingAStar.GeneratePath(_mapCell->GetArrayPosition(),
			_playerCharacter->GetMapCell()->GetArrayPosition(), 
			std::vector<CellState>(enemyPassableCells, enemyPassableCells + 2));
		// Move to the player
		SetCellObjectPosition(*_pathToPlayer.begin()); 
		// Update path
		_pathToPlayer.pop_front();
	}
	// Else, just pass the turn
	else
	{
		UseTurnPoint();
	}
}

const CellState EnemyCharacter::enemyPassableCells[2] = { CellState::Free, CellState::Occupied };