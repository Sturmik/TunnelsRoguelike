#pragma once

#include "MapCell.h"
#include <iostream>
#include <algorithm>
#include <limits.h>
#include <list>
#include <vector>
#include <map>

// Placement enum
enum DoorPlacement
{
	Top,
	Right,
	Bottom,
	Left
};

// Room class
class Room
{
private:
	// Struct door for room
	struct Door
	{
		// Door position
		Point2DInt position;
		// Door placement
		DoorPlacement placement;
		// Connected room
		Room* connectedRoom;

		Door(Point2DInt position, DoorPlacement placement) :
			position(position), placement(placement), connectedRoom(nullptr) {}
	};

	// Room doors
	std::vector<Door> _doors;
	// Left top point
	Point2DInt _leftTopPosition;
	// Right bottom point
	Point2DInt _rightBottomPosition;

	// Checks, if has way to room
	bool HasWayToRoom(Room& destinationRoom, std::vector<Room*>& visitedRooms);
public:
	// Constructors
	Room() {}
	Room(Point2DInt leftTopPosition, Point2DInt rightBottomPosition)
	{
		if (rightBottomPosition.x < leftTopPosition.x) { rightBottomPosition.x = leftTopPosition.x; }
		if (rightBottomPosition.y < leftTopPosition.y) { rightBottomPosition.y = leftTopPosition.y; }
		_leftTopPosition = leftTopPosition;
		_rightBottomPosition = rightBottomPosition;
	}
	// Copy-constructor
	Room(const Room& room) : _doors(room._doors), _leftTopPosition(room._leftTopPosition),
		_rightBottomPosition(room._rightBottomPosition) {}

	// Gets door position on placement
	// If there is no such door exist, return it's potential position
	Point2DInt GetDoorPositionOnPlacement(DoorPlacement doorPlacement);
	// Checks, if there is door on specific placement
	bool HasDoorOnPlacement(DoorPlacement doorPlacement);
	// Gets closest door to given position
	Point2DInt GetClosestDoorPositionToPosition(Point2DInt position);
	// Add new door. Returns true, if operation was successful
	bool AddDoor(Point2DInt doorPos, Room* connectedTo);
	// Removes door from specific placement. Returns true, if operation was successful
	bool RemoveDoor(DoorPlacement doorPlacement);

	// Gets doors vector
	std::vector<Door> GetDoors() { return _doors; }
	// Gets left top position
	Point2DInt GetLeftTopPosition() { return _leftTopPosition; }
	// Gets right bottom position
	Point2DInt GetRightBottomPosition() { return _rightBottomPosition; }
	// Get center of the room
	Point2DInt GetCenterOfTheRoomPosition() const {
		return Point2DInt((_leftTopPosition.x + _rightBottomPosition.x) / 2,
		(_leftTopPosition.y + _rightBottomPosition.y) / 2); }
	// Get size of the room (x - width, y - height)
	Point2DInt GetSizeOfRoom();

	// Does our room intersect with other room 
	bool DoesIntersectWithRoom(Room intersectRoom, int spaceBetweenRooms = 1);
	// Checks, if you can get to the given room, by using doors
	bool HasWayToRoom(Room& destinationRoom);
};

// Creates 2D array
template <class T>
T** CreateArr2D(T** array2D, int rows, int columns)
{
	array2D = new T * [rows];
	for (int row = 0; row < rows; row++)
	{
		array2D[row] = new T[columns];
	}
	return array2D;
}

// Creates 2D array
template <class T>
void DeleteArr2D(T** array2D, int rows, int columns)
{
	for (int row = 0; row < rows; row++)
	{
		delete[] array2D[row];
	}
	delete[] array2D;
}

class Map
{	
private:
	// Map attributes
	int _mapWidth;
	int _mapHeight;
	MapCell** _map;
	// Vector of rooms on map
	std::vector<Room> _rooms;
public:
	// Constructor
	Map(int mapWidth, int mapHeight) : _mapWidth(mapWidth), _mapHeight(mapHeight)
	{
		if (_mapWidth < 0) { _mapWidth = 0; }
		if (_mapHeight < 0) { _mapHeight = 0; }
		_map = CreateArr2D(_map, _mapHeight, _mapWidth);
	}
	// Deconstructor
	~Map()
	{
		// Delete map
		DeleteArr2D(_map, _mapHeight, _mapWidth);
		// Clear rooms
		_rooms.clear();
	}
	// Gets map
	MapCell** GetMap2D() { return _map; }
	// Gets rooms
	std::vector<Room>& GetRooms() { return _rooms; }
	// Get map width
	int GetMapWidth() { return _mapWidth; }
	// Get map height
	int GetMapHeight() { return _mapHeight; }
};

// Double max value
const double DOUBLE_MAX = std::numeric_limits<double>::max();
// Class for A* path finding
class PathFindingAStar
{
private:
	// Node structure for proper work
	class ANode
	{
	private:
		// Node position
		Point2DInt _position;
		// Parent of node
		ANode* _parent;
		// Node neighbours
		std::vector<ANode*> _neighbours;
		// Is node an obstacle
		bool _isObstacle;
		// Checks, if node is open 
		bool _isOpen;
		// Distance formed from previous node
		double _localDistance;
		// Distance to the end point
		double _distanceToTheEndPoint;
	public:
		// Getters

		Point2DInt GetPosition() const { return _position; }
		ANode* GetParent() const { return _parent; }
		std::vector<ANode*>& GetNeighbours() { return _neighbours; }
		bool IsObstacle() const { return _isObstacle; }
		bool IsOpen() const { return _isOpen; }
		double GetLocalDistance() const { return _localDistance; }
		double GetDistanceToTheEndPoint() const { return _distanceToTheEndPoint; }
		double GetGlobalDistance() const
		{
			if (_localDistance == DOUBLE_MAX || _distanceToTheEndPoint == DOUBLE_MAX) { return DOUBLE_MAX; }
			return _localDistance + _distanceToTheEndPoint;
		}

		// Setters

		void SetPosition(Point2DInt position) { _position = position; }
		void SetParent(ANode* parent) { _parent = parent; }
		void SetObstacleState(bool isObstacle) { _isObstacle = isObstacle; }
		void SetOpenState(bool openState) { _isOpen = openState; }
		void SetLocalDistance(double localDistance) { _localDistance = localDistance; }
		void SetDistanceToTheEndPoint(double distanceToTheEndPoint) { _distanceToTheEndPoint = distanceToTheEndPoint; }

		// Set node to default values
		void SetNodeToDefaultValues()
		{
			_parent = nullptr;
			_isObstacle = false;
			_isOpen = false;
			_localDistance = DOUBLE_MAX;
			_distanceToTheEndPoint = DOUBLE_MAX;
		}

		// Default constructor
		ANode() { SetNodeToDefaultValues(); }
	};

	// Map
	Map* _map;
	// Node map
	ANode** _aNodeMap;

	// Checks, if cell is passable
	bool IsCellPassable(std::vector<CellState>& passableCells, Point2DInt cellPos);

	// Forming 2D array of nodes
	void FormNodeMap(Point2DInt startPoint, Point2DInt endPoint,
		std::vector<CellState>& passableCells)
	{
		// Edit every node according to given data
		for (int row = 0; row < _map->GetMapHeight(); row++)
		{
			for (int col = 0; col < _map->GetMapWidth(); col++)
			{
				// Set node to default values
				_aNodeMap[row][col].SetNodeToDefaultValues();
				// Setting position
				_aNodeMap[row][col].SetPosition(Point2DInt(col, row));
				// Setting distances
				_aNodeMap[row][col].SetDistanceToTheEndPoint(endPoint.Distance(Point2DInt(col, row)));
				// Setting neighbours
				if (IsCellPassable(passableCells, Point2DInt(col, row)))
				{
					if (_aNodeMap[row][col].GetNeighbours().size() == 0)
					{
						// Add north neighbour
						if (row > 0)
						{
							_aNodeMap[row][col].GetNeighbours().push_back(&_aNodeMap[row - 1][col]);
						}
						// Add south neighbour
						if (row < _map->GetMapHeight() - 1)
						{
							_aNodeMap[row][col].GetNeighbours().push_back(&_aNodeMap[row + 1][col]);
						}
						// Add west neighbour
						if (col > 0)
						{
							_aNodeMap[row][col].GetNeighbours().push_back(&_aNodeMap[row][col - 1]);
						}
						// Add east neighbour
						if (col < _map->GetMapWidth() - 1)
						{
							_aNodeMap[row][col].GetNeighbours().push_back(&_aNodeMap[row][col + 1]);
						}
					}
				}
				else
				{
					// Mark cell as obstacle
					_aNodeMap[row][col].SetObstacleState(true);
				}
				// If, we are at the start point 
				if (row == startPoint.y && col == startPoint.x)
				{
					// Set its local distance to zero
					_aNodeMap[row][col].SetLocalDistance(0);
				}
			}
		}
	}
public:
	PathFindingAStar(Map* map) : _map(map)
	{
		// Create two dimensional array
		_aNodeMap = CreateArr2D(_aNodeMap, map->GetMapHeight(), map->GetMapWidth());
	}

	~PathFindingAStar()
	{
		// Delete array
		DeleteArr2D(_aNodeMap, _map->GetMapHeight(), _map->GetMapWidth());
	}

	// Generates shortest path
	std::vector<Point2DInt> GeneratePath(Point2DInt startPoint, Point2DInt endPoint,
		std::vector<CellState>& passableCells);
};

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
	// Generate Tunnel between two points, using wave algorithm
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

	~MapGenerator()
	{
		delete _pathFinding;
	}

	// Generates map
	void GenerateMap(int numberOfRooms)
	{
		// Clear all map
		ClearMap();
		// Generate rooms
		GenerateRooms(numberOfRooms);
		// Generate room doors and connections between them
		if (numberOfRooms > 0)
		{
			GenerateRoomConnections();
		}
		// While, tunnels were generated, all doors were marked as occupied to 
		// prevent A* start algorithm to build tunnels inside rooms.
		// That's why, after the room connections generation was finished,
		// update all door cells states to "OPEN"
		for (int room = 0; room < _map->GetRooms().size(); room++)
		{
			for (int door = 0; door < _map->GetRooms()[room].GetDoors().size(); door++)
			{
				// Get door position
				Point2DInt doorPos = _map->GetRooms()[room].GetDoors()[door].position;
				// Update cell state to free
				_map->GetMap2D()[doorPos.y][doorPos.x].SetCellState(CellState::Free);
			}
		}
	}
};