#pragma once

#include "InteractiveObject.h"

// Trigger is calling specific instruction
class Trigger : public InteractiveObject
{
protected:
	Trigger(Map* map,MapCell* mapCell, 
		std::string name)
		: InteractiveObject(map, mapCell, name) {}
public:
	ObjectType GetObjectType() const override 
	{ return ObjectType::TriggerObject; }

	// Invoke specific instruction
	virtual void Invoke() = 0;
};

// Ladder 
class Ladder : public Trigger
{
protected:
	void OnPositionChange() override
	{
		// Update cell state to invocable
		_mapCell->SetCellState(CellState::Invocable);
	};
public:
	Ladder(Map* map, MapCell* mapCell)
		: Trigger(map, mapCell, "Ladder")
	{}

	// Invokes next level
	void Invoke() override { /*call nextLevel*/ }
};