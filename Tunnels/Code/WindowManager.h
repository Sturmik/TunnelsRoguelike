#include "GameObject.h"
#include <iostream>
#include <vector>

// Window manager class for handling window operation and events
class WindowManager
{
private:
#pragma region Variables
	// Main window
	sf::RenderWindow _window;
	// List of objects to draw
	std::vector<GameObject> _gameObjects;
#pragma endregion
#pragma region Methods
	// Start window and loop in it
	void Update();
#pragma endregion
public:
#pragma region ConstructorDeconstructor
	WindowManager();
	// Default deconstructor
	~WindowManager();
#pragma endregion 
#pragma region Methods
	// Open window and loop in it
	void OpenWindow(sf::VideoMode videoMode, std::string name);
	// Add drawable object
	void AddObject(GameObject& newObject);
	// Delete drawable object
	void RemoveObject(GameObject& objectToRemove);
#pragma endregion
};