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
    // Start timer
    UtilityTime::FrameStartTime();
    // Process events
    sf::Event windowEvent;
    while (_window.pollEvent(windowEvent))
    {
        if (windowEvent.type == sf::Event::Resized)
        {
            sf::FloatRect visibleArea(0, 0, windowEvent.size.width, windowEvent.size.height);
            _window.setView(sf::View(visibleArea));
        }
        // Send message to all event listeners
        for (std::list<WindowEventListener*>::iterator listener = _eventListeners.begin();
            listener != _eventListeners.end(); listener++)
        {
            (**listener).WindowEventProcess(_window, windowEvent);
        }
    }
    windowEvent.type = sf::Event::EventType(-1);
    // Send message to all event listeners (without event)
    for (std::list<WindowEventListener*>::iterator listener = _eventListeners.begin();
        listener != _eventListeners.end(); listener++)
    {
        (**listener).WindowEventProcess(_window, windowEvent);
    }
    // Clear window
    _window.clear();
    // Draw all objects
    for (int layer = Layer::BackLayer; layer <= Layer::InterfaceLayer; layer++)
    {
        std::list<sf::Drawable*>::iterator lastObject = _gameObjectsLayers[static_cast<Layer>(layer)].begin();
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
                if (gmObj->IsObjectDead()) 
                {
                    RemoveObject(gmObj);
                    // After we remove object - update iterator
                    object = _gameObjectsLayers[static_cast<Layer>(layer)].begin();
                }
            }
            lastObject = object;
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
                return;
            }
        }
    }
}

void WindowManager::AddEventListener(WindowEventListener* eventListener)
{
    _eventListeners.push_back(eventListener);
}

void WindowManager::RemoveListener(WindowEventListener* eventListener)
{
    _eventListeners.remove(eventListener);
}