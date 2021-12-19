#pragma once

#include "EnemyCharacter.h"

// Light enemy
// - Has many turn points
// - Weak strength
// - Low health
class LightEnemyCharacter : public EnemyCharacter
{
private:
public:
	LightEnemyCharacter(PlayerCharacter* playerCharacter, Map* map,
		MapCell* mapCell) : EnemyCharacter(playerCharacter, map, mapCell,
			"Light enemy", 5, 3, 3, 3, 5)
	{
		// Load texture
		SetTexture("Textures\\EnemyLight.png");
	}
};