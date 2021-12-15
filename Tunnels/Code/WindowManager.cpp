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
    sf::Event windowEvent;
    while (_window.pollEvent(windowEvent))
    {
        // Send message to all event listeners
        for (std::list<EventListener*>::iterator listener = _eventListeners.begin();
            listener != _eventListeners.end(); listener++)
        {
            (**listener).EventProcess(windowEvent);
        }
        if (windowEvent.type == sf::Event::Resized)
        {
            sf::FloatRect visibleArea(0, 0, windowEvent.size.width, windowEvent.size.height);
            _window.setView(sf::View(visibleArea));
        }
    }
    // Clear window
    _window.clear();
    // Draw all objects
    for (int layer = Layer::BackLayer; layer <= Layer::InterfaceLayer; layer++)
    {
        for (std::list<sf::Drawable*>::iterator object = _gameObjectsLayers[static_cast<Layer>(layer)].begin();
            object != _gameObjectsLayers[static_cast<Layer>(layer)].end(); object++)
        {
            // Draw object
            _window.draw(**object);
            // Try to cast object to game object
            GameObject* gmObj = dynamic_cast<GameObject*>(*object);
            // If it is game object - update its state
            if (gmObj != nullptr)
            {
                gmObj->Update();
                // Check, if object is dead remove it from game objects map
                if (gmObj->IsObjectDead()) { RemoveObject(gmObj); }
            }
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

void WindowManager::AddObject(Layer layer, sf::Drawable* newObject)
{
    _gameObjectsLayers[layer].push_back(newObject);
}

void WindowManager::RemoveObject(sf::Drawable* objectToRemove)
{
    for (int layer = Layer::BackLayer; layer < Layer::InterfaceLayer; layer++)
    {
        Layer objectLayer = static_cast<Layer>(layer);
        for (std::list<sf::Drawable*>::iterator object = _gameObjectsLayers[static_cast<Layer>(layer)].begin();
            object != _gameObjectsLayers[static_cast<Layer>(layer)].end(); object++)
        {
            if (*object == objectToRemove)
            {
                _gameObjectsLayers[objectLayer].erase(object);
                continue;
            }
        }
    }
}

void WindowManager::AddEventListener(EventListener* eventListener)
{
    _eventListeners.push_back(eventListener);
}

void WindowManager::RemoveListener(EventListener* eventListener)
{
    _eventListeners.remove(eventListener);
}