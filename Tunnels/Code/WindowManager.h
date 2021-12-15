#pragma once

#include "GameObject.h"
#include <iostream>
#include <map>
#include <list>

// Interface for event process
// It allows classes, which implement this interface
// to get messages about events in window
class EventListener
{
public:
	// Keyboard input process
	virtual void EventProcess(sf::Event windowEvent) = 0;
};

// Layers
enum Layer
{
	BackLayer,
	FrontLayer,
	InterfaceLayer
};

// Window manager class for handling window operation and events
class WindowManager
{
private:
	// Main window
	sf::RenderWindow _window;
	// Linked list of input listeners
	std::list<EventListener*> _eventListeners;
	// List of objects to draw
	std::map < Layer, std::list < sf::Drawable * >> _gameObjectsLayers;
public:
	WindowManager();
	// Default deconstructor
	~WindowManager();

	// Open window and loop in it
	void OpenWindow(sf::VideoMode videoMode, std::string name);
	// Add drawable object
	void AddObject(Layer layer, sf::Drawable* newObject);
	// Delete drawable object
	void RemoveObject(sf::Drawable* objectToRemove);
	// Adds listener
	void AddEventListener(EventListener* eventListener);
	// Remove listener
	void RemoveListener(EventListener* eventListener);
	// Update everything in window
	void Update();
};