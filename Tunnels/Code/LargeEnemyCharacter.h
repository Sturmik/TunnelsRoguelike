#pragma once

#include "EnemyCharacter.h"

// Large enemy
// - Has small amount of turn points
// - Large strength
// - Large health
class LargeEnemyCharacter : public EnemyCharacter
{
public:
	LargeEnemyCharacter(PlayerCharacter* playerCharacter, Map* map,
		MapCell* mapCell) : EnemyCharacter(playerCharacter, map, mapCell,
			"Large enemy", 3, 10, 10, 10, 15)
	{
		// Load texture
		SetTexture("Textures\\EnemyLarge.png");
	}
};