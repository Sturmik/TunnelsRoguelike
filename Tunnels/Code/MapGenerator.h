#pragma once

#include "MapCell.h"
#include <algorithm>
#include <vector>
#include <map>

// Vector struct, represents x and y
struct Vector2Int
{
	int x, y;
	Vector2Int() { x = 0; y = 0; }
	Vector2Int(int x, int y) : x(x), y(y) {}

	// Calculates distance between recent and given position
	double Distance(Vector2Int vector)
	{
		return sqrt(pow(x - vector.x, 2) + pow(y - vector.y, 2));
	}
};

// Placement enum
enum Placement
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
		Vector2Int position;
		// Door placement
		Placement placement;
		// Connected room
		Room* connectedRoom;

		Door(Vector2Int position, Placement placement) :
			position(position), placement(placement), connectedRoom(nullptr) {}
	};

	// Room doors
	std::vector<Door> _doors;
	// Left top point
	Vector2Int _leftTopPosition;
	// Right bottom point
	Vector2Int _rightBottomPosition;
public:
	// Constructors
	Room() {}
	Room(Vector2Int leftTopPosition, Vector2Int rightBottomPosition)
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
	Vector2Int GetDoorPositionOnPlacement(Placement doorPlacement);
	// Checks, if there is door on specific placement
	bool HasDoorOnPlacement(Placement doorPlacement);
	// Gets closest door to givent position
	Door GetClosestDoorToPosition(Vector2Int position);
	// Add new door. Returns true, if operation was successful
	bool AddDoor(Placement doorPlacementPos, int doorPos = -1);
	// Removes door from specific placement. Returns true, if operation was successful
	bool RemoveDoor(Placement doorPlacement);

	// Move room
	void Move(int dx, int dy = 0);
	// Gets doors vector
	std::vector<Door> GetDoors() { return _doors; }
	// Gets left top position
	Vector2Int& GetLeftTopPosition() { return _leftTopPosition; }
	// Gets right bottom position
	Vector2Int& GetRightBottomPosition() { return _rightBottomPosition; }
	// Get center of the room
	Vector2Int GetCenterOfTheRoomPosition() const { return Vector2Int(_rightBottomPosition.x / 2, _rightBottomPosition.y / 2); }
	// Get size of the room (x - width, y - height)
	Vector2Int GetSizeOfRoom();

	// Does our room intersect with other room 
	bool DoesIntersectWithRoom(Room intersectRoom, int spaceBetweenRooms = 1);
	// Checks, if you can get to the given room, by using doors
	bool HasWayToRoom(Room& room);
};

class PathFindingAStar
{
private:
	class ANode
	{
	private:
		double _distanceToStartPoint;
		double _distanceToTheEndPoint;
		double _totalCost;
	public:
		ANode(Vector2Int recentPoint, Vector2Int startPoint, Vector2Int endPoint)
		{
			_distanceToStartPoint = recentPoint.Distance(startPoint) * 10;
			_distanceToTheEndPoint = recentPoint.Distance(endPoint) * 10;
			_totalCost = _distanceToStartPoint + _distanceToTheEndPoint;
		}

		double GetDistanceToStartPoint() const { return _distanceToStartPoint; }
		double GetDistanceToTheEndPoint() const { return _distanceToTheEndPoint; }
		double GetTotalCost() const { return _totalCost; }
	};
public:
	// Generates shortest path
	std::vector<Vector2Int> GeneratePath(Vector2Int startPoint, Vector2Int endPoint,
		MapCell** map, int mapWidth, int mapHeight)
	{

	}
};

class MapGenerator
{
private:
	// Map attributes
	int _mapWidth;
	int _mapHeight;
	MapCell** _map;
	std::vector<Room> _rooms;

	// Creates 2D array
	MapCell** CreateArr2D(MapCell** array2D, int rows, int columns);
	// Draws rooms on the 2D array
	void DrawRoom(MapCell** array2D, Room& room);
	// Returns true, if room was successfuly added
	bool AddRoom(Room newRoom);
	// Checks if it possible to place the room on the map
	bool CheckRoomForMapBoundaries(Room newRoom);
	// Checks if room can fit on the map
	bool DoesRoomFitInMap(Room newRoom);
	// Generate rooms
	void GenerateRooms(int numberOfRooms);
	// Gets vector of closest rooms towards specific room
	std::vector<Room> GetClosestRoomsToPosition(Vector2Int position, Room& roomToIgnore);
	// Generate Tunnel between two points, using wave algorithm
	void GenerateTunnel(Vector2Int startPoint, Vector2Int endPoint);
	// Generate room doors
	void GenerateRoomConnections();
public:
	// Default constructor
	MapGenerator(int mapWidth, int mapHeight) : _mapWidth(mapWidth), _mapHeight(mapHeight)
	{
		if (mapWidth < 0) { mapWidth = 0; }
		if (mapHeight < 0) { mapHeight = 0; }
		_map = CreateArr2D(_map, _mapHeight, _mapWidth);
	}
	// Default deconstructor
	~MapGenerator()
	{
		// Clear map
		for (int i = 0; i < _mapHeight; i++)
		{
			delete[] _map[i];
		}
		delete[] _map;
		// Clear rooms
		_rooms.clear();
	}

	// Get map
	MapCell** GetMap() { return _map; }
	// Get map width
	int GetMapWidth() { return _mapWidth; }
	// Get map height
	int GetMapHeight() { return _mapHeight; }
	// Generates map
	void GenerateMap(int numberOfRooms)
	{
		// Clear all previous rooms
		_rooms.clear();
		// Generate rooms
		GenerateRooms(numberOfRooms);
		// Generate room doors and connections between them
		GenerateRoomConnections();
	}
};