#pragma once

#include "EventListener.h"
#include "InteractiveObject.h"

// Trigger is calling specific instruction
class Trigger : public InteractiveObject
{
protected:
	void OnPositionChange() override
	{
		// Update cell state to invocable
		_mapCell->SetCellState(CellState::Invokable);
	};
	// Trigger listener list
	std::list<MethodEventListener*> _triggerListenersList;
	// Constructor
	Trigger(Map* map,MapCell* mapCell, 
		std::string name)
		: InteractiveObject(map, mapCell, name) {}
public:
	ObjectType GetObjectType() const override 
	{ return ObjectType::TriggerObject; }

	// Adds listener
	void AddEventListener(MethodEventListener* eventListener)
	{
		_triggerListenersList.push_back(eventListener);
	}

	// Remove listener
	void RemoveListener(MethodEventListener* eventListener)
	{
		_triggerListenersList.push_back(eventListener);
	}

	// Invoke specific instruction
	void Invoke()
	{
		// Send message to all event listeners (without event)
		for (std::list<MethodEventListener*>::iterator listener = _triggerListenersList.begin();
			listener != _triggerListenersList.end(); listener++)
		{
			(**listener).EventProcess(this);
		}
	}
};