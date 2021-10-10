#include "WindowManager.h"

WindowManager::WindowManager() :_window(), _gameObjects()
{
    // Initialize
}

WindowManager::~WindowManager()
{
	if (_window.isOpen())
	{
		_window.close();
	}
}

void WindowManager::Update()
{
    while (_window.isOpen())
    {
        sf::Event event;
        while (_window.pollEvent(event))
        {
            // Input here
        }
        // Clear window
        _window.clear();
        // Draw all objects
        for (int i = 0; i < _gameObjects.size(); i++)
        {
            _window.draw(_gameObjects[i]);
        }
        // Display everything on window
        _window.display();
    }
}

void WindowManager::OpenWindow(sf::VideoMode videoMode, std::string name)
{
	_window.create(videoMode, name);
	Update();
}

void WindowManager::AddObject(GameObject& newObject)
{
    _gameObjects.push_back(newObject);
}

void WindowManager::RemoveObject(GameObject& objectToRemove)
{
    for (int i = 0; i < _gameObjects.size(); i++)
    {
        if (&_gameObjects[i] == &objectToRemove)
        { 
            _gameObjects.erase(_gameObjects.begin() + i);
            return;
        }
    }
}