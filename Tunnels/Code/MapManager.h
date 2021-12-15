#include "MapGenerator.h"
#include "InterfaceManager.h"

// Map manager controls almost everything what is related to gameplay
// - Map generation
// - Object moving
// - Player object control
class MapManager : EventListener
{
private:
	// Default constant size of the map
	const int MAP_DEFAULT_SIZE_X = 120;
	const int MAP_DEFAULT_SIZEE_Y = 60;

	// Map with game objects
	Map _map;
	
	// Window manager
	WindowManager* _windowManager;
	// Interface manager pointer for interface updates
	InterfaceManager* _interfaceManager;
public:
	MapManager(WindowManager* windowManager, InterfaceManager* interfaceManager) : 
	_map(MAP_DEFAULT_SIZE_X, MAP_DEFAULT_SIZEE_Y), 
		_windowManager(windowManager), _interfaceManager(interfaceManager)
	{
		// "Subscribe" to window manager, to get messages about events
		_windowManager->AddEventListener(this);
		// Push all elements in window manager
		for (int i = 0; i < _map.GetMapHeight(); i++)
		{
			for (int j = 0; j < _map.GetMapWidth(); j++)
			{
				_windowManager->AddObject(Layer::BackLayer, &_map.GetMap2D()[i][j]);
			}
		}
	}

	~MapManager() { _windowManager->RemoveListener(this); }

	// Updates state of the program
	void Update();
	// Generates map with all objects on it
	void GenerateMap(int numberOfRooms);

	// For window event processing
	virtual void EventProcess(sf::Event windowEvent) override;
};