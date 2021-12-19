#pragma once

#include "PlayerCharacter.h"
// Path finding for chasing player
#include "PathFindingAStar.h"
// For sleep function
#include <windows.h>

// Abstract enemy class
class EnemyCharacter : public BaseCharacter
{
protected:
	// Passable cells for enemy character
	static const CellState ENEMY_PASSABLE_CELLS_ARRAY[2];
	// Defines radius, where enemy will see player and attack him
	const float ENEMY_VIEW_RADIUS = 50.0f;

	// Path finding
	PathFindingAStar _pathFindingAStar;
	// Path to player
	std::list <Point2DInt> _pathToPlayer;

	// Pointer to player allows enemy to calculate distance to the player
	PlayerCharacter* _playerCharacter;

	// Character interaction
	void CharacterInteraction(BaseCharacter* charObj) override;
	// Trigger interaction
	void TriggerInteraction(Trigger* triggerObj) override;

	EnemyCharacter(PlayerCharacter* playerCharacter, Map* map,
		MapCell* mapCell, std::string name, int maxWalkPoints, int health,
		int armor, int strength, int goldCount) : _pathFindingAStar(map),
		_playerCharacter(playerCharacter), BaseCharacter(map, mapCell, name, maxWalkPoints,
			health, armor, strength, goldCount) {}
public:
	// Updates state of enemy
	void Update() override;

	// Get character type
	CharacterType GetCharacterType() const override { return CharacterType::EnemyType; }
};