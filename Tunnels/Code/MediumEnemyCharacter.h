#pragma once

#include "EnemyCharacter.h"

// Medium enemy
// - Has average amount of turn points
// - Medium strength
// - Medium health
class MediumEnemyCharacter : public EnemyCharacter
{
public:
	MediumEnemyCharacter(PlayerCharacter* playerCharacter, Map* map,
		MapCell* mapCell) : EnemyCharacter(playerCharacter, map, mapCell,
			"Medium enemy", 4, 5, 5, 5, 10)
	{
		// Load texture
		SetTexture("Textures\\EnemyMedium.png");
	}
};