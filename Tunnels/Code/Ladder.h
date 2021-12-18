#pragma once

#include "Trigger.h"

// Ladder 
class Ladder : public Trigger
{
public:
	Ladder(Map* map, MapCell* mapCell)
		: Trigger(map, mapCell, "Ladder")
	{
		// Load texture
		SetTexture("Textures\\Ladder.png");
	}
};