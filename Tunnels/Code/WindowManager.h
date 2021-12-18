#pragma once

#include "UtilityTime.h"
#include "EventListener.h"
#include <iostream>
#include <map>
#include <list>

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
	std::list<WindowEventListener*> _eventListeners;
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
	void AddEventListener(WindowEventListener* eventListener);
	// Remove listener
	void RemoveListener(WindowEventListener* eventListener);
	// Update everything in window
	void Update();
};