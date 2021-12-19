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
	// If object is dead, it can't do anything anymore
	if (IsObjectDead()) { _recentTurnPoints = 0; return; }
	// If map cell or player were not assigned - return
	if (_mapCell == nullptr || _playerCharacter == nullptr) { return; }
	// If player character's mapcell was not assigned, or there are no turns left - return
	if (_playerCharacter->GetMapCell() == nullptr || GetRecentTurnPoints() <= 0) { return; }
	// Caclculate distance to player
	float distanceToPlayer = _mapCell->GetArrayPosition().Distance(_playerCharacter->GetMapCell()->GetArrayPosition());
	// If player is nearby and there are no turn points left - try to use turn potion
	if (distanceToPlayer < ENEMY_VIEW_RADIUS && GetRecentTurnPoints() <= 1) { UseRecentTurnPotion();  }
	// If health if lesser than half of full - heal
	if (_recentHealth < _maxHealth / 2) { UseRecentHealPotion(); }
	// Target map cell for object interaction
	MapCell* targetMapCell = nullptr;
	// If there is player nearby - chase and fight him
	// If player is inside of the enemy view radius - form way to him
	if (distanceToPlayer < ENEMY_VIEW_RADIUS)
	{
		_pathToPlayer = _pathFindingAStar.GeneratePath(_mapCell->GetArrayPosition(),
			_playerCharacter->GetMapCell()->GetArrayPosition(), 
			std::vector<CellState>(ENEMY_PASSABLE_CELLS_ARRAY, ENEMY_PASSABLE_CELLS_ARRAY + 2));
		// Pop front element, because it is recent enemy position
		_pathToPlayer.pop_front();
	}
	// Go by the path
	if (_pathToPlayer.size() > 0)
	{
		if (SetCellObjectPosition(*_pathToPlayer.begin(), targetMapCell))
		{
			// If movement was successful, pop front element, because it is recent enemy position
			_pathToPlayer.pop_front();
		}
		else
		{
			// Pass the turn
			UseTurnPoint();
		}
	}
	else
	{
		// Pass the turn
		UseTurnPoint();
	}
	// Interact with map cell, if there is any object on it
	if (targetMapCell != nullptr)
	{
		Interact(dynamic_cast<InteractiveObject*>(targetMapCell->GetGameObject()));
	}
}

const CellState EnemyCharacter::ENEMY_PASSABLE_CELLS_ARRAY[2] = { CellState::Free, CellState::Occupied };