#pragma once

#include "PathFindingAStar.h"

#include <iostream>

// Class for generating objects on map
class MapGenerator
{
private:
	// Map border offset
	const int MAP_BORDER_OFFSET = 2;

	// Map
	Map* _map;
	// Path finding class
	PathFindingAStar* _pathFinding;

	// Textures for map generating
	sf::Texture _wallTexture, _floorTexture, _doorTexture, _tunnelTexture;

	// Sets all mapcells to default state
	void ClearMap();

	// Adds room on map
	bool AddRoom(Room newRoom);
	// Checks if it possible to place the room on the map
	bool CheckRoomForMapBoundaries(Room newRoom);
	// Checks if room can fit on the map
	bool DoesRoomFitInMap(Room newRoom);
	// Generate rooms
	void GenerateRooms(int numberOfRooms);
	// Gets vector of closest rooms towards specific room
	std::vector<Room*> GetClosestRoomsToPosition(Point2DInt position, Room& roomToIgnore);
	// Generate Tunnel between two points, using A* algorithm
	void GenerateTunnel(Room* startRoom, Point2DInt startPoint, Room* endRoom, Point2DInt endPoint);
	// Generate room doors
	void GenerateRoomConnections();
public:
	// Default constructor
	MapGenerator(Map* map)
	{ 
		// Getting map
		_map = map; 
		// Forming pathfinding
		_pathFinding = new PathFindingAStar(map);
		// Loading texture
		try
		{
			_floorTexture.loadFromFile("Textures\\CellOpen.png");
			_wallTexture.loadFromFile("Textures\\CellWall.png");
			_doorTexture.loadFromFile("Textures\\CellDoor.png");
			_tunnelTexture.loadFromFile("Textures\\CellTunnel.png");
		}
		catch (const char* str)
		{
			std::cout << "ERROR: " << str;
		}
	}

	// Deconstructor
	~MapGenerator()
	{
		delete _pathFinding;
	}

	// Generates map
	void NextMap(int numberOfRooms);
};