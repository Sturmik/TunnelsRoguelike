#include "WindowManager.h"

WindowManager::WindowManager() :_window(), _gameObjectsLayers()
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
    sf::Event event;
    while (_window.pollEvent(event))
    {
        // Input here
        if (event.type == sf::Event::Resized)
        {
            sf::FloatRect visibleArea(0, 0, event.size.width, event.size.height);
            _window.setView(sf::View(visibleArea));
        }
    }
    // Clear window
    _window.clear();
    // Draw all objects
    for (int layer = Layer::BackLayer; layer < Layer::InterfaceLayer; layer++)
    {
        for (int object = 0; object < _gameObjectsLayers[static_cast<Layer>(layer)].size(); object++)
        {
            if (_gameObjectsLayers[static_cast<Layer>(layer)][object] == nullptr)
            {
                _gameObjectsLayers[static_cast<Layer>(layer)].erase(_gameObjectsLayers[static_cast<Layer>(layer)].begin() + object);
                continue;
            }
            _window.draw(*_gameObjectsLayers[static_cast<Layer>(layer)][object]);
        }
    }
    // Display everything on window
    _window.display();
}

void WindowManager::OpenWindow(sf::VideoMode videoMode, std::string name)
{
    if (!_window.isOpen())
    {
        _window.create(videoMode, name);
    }
}

void WindowManager::AddObject(Layer layer, GameObject* newObject)
{
    _gameObjectsLayers[layer].push_back(newObject);
}

void WindowManager::RemoveObject(GameObject* objectToRemove)
{
    for (int layer = Layer::BackLayer; layer < Layer::InterfaceLayer; layer++)
    {
        Layer objectLayer = static_cast<Layer>(layer);
        for (int object = 0; object < _gameObjectsLayers[objectLayer].size(); object++)
        {
            if (_gameObjectsLayers[objectLayer][object] == objectToRemove)
            {
                _gameObjectsLayers[objectLayer].erase(_gameObjectsLayers[objectLayer].begin() + object);
                continue;
            }
        }
    }
}