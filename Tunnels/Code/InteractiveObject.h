#pragma once

#include "CellObject.h"

// Object types
enum class ObjectType
{
	// Active objects in the game.
	// They can move, store items and 
	// be the ones who initiate the interaction
	CharacterObject,
	// Passive objects in the game.
	// They can be picked up by characters objects
	// and used by them
	ItemObject,
	// Specific objects in the game.
	// Executed in certain scenario.
	TriggerObject
};

// Interactive object is a base class for all 
// objects which are involved in any kind of communication
class InteractiveObject : public CellObject
{
protected:
	// Object name
	std::string _objectName;

	InteractiveObject(Map* map,
		MapCell* mapCell, std::string name)
		: CellObject(map, mapCell)
	{ 
		_objectName = name;
	}
public:
	// Gets object name
	std::string GetObjectName() const { return _objectName; }
	// Virtual function for object defining
	virtual ObjectType GetObjectType() const = 0;
};