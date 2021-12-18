#pragma once

#include "GameObject.h"

// Interface for event process
// It allows classes, which implement this interface
// to get messages about event occuring in specific classes
class MethodEventListener
{
public:
	// Event processing
	virtual void EventProcess(GameObject* gameObjectEvent) = 0;
};

// Interface for event process
// It allows classes, which implement this interface
// to get messages about events in window
class WindowEventListener
{
public:
	// Event processing
	virtual void WindowEventProcess(sf::RenderWindow& window, sf::Event windowEvent) = 0;
};