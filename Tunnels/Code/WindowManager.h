#pragma once

#include "GameObject.h"
#include <iostream>
#include <vector>

// Window manager class for handling window operation and events
class WindowManager
{
private:
	// Main window
	sf::RenderWindow _window;
	// List of objects to draw
	std::vector<GameObject*> _gameObjects;
public:
	WindowManager();
	// Default deconstructor
	~WindowManager();

	// Open window and loop in it
	void OpenWindow(sf::VideoMode videoMode, std::string name);
	// Add drawable object
	void AddObject(GameObject& newObject);
	// Delete drawable object
	void RemoveObject(GameObject& objectToRemove);
	// Update everything in window
	void Update();
};