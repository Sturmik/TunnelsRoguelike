#pragma once

#include "MapCell.h"

#include <list>
#include <vector>
#include <map>
#include <algorithm>

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

// Placement enum
enum PlacementDirection
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
		// Door doorPlacement
		PlacementDirection doorPlacement;
		// Connected room
		Room* connectedRoom;

		Door(Point2DInt position, PlacementDirection doorPlacement) :
			position(position), doorPlacement(doorPlacement), connectedRoom(nullptr) {}
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

	// Gets door position on doorPlacement
	// If there is no such door exist, return it's potential position
	Point2DInt GetDoorPositionOnPlacement(PlacementDirection doorPlacement);
	// Checks, if there is door on specific doorPlacement
	bool HasDoorOnPlacement(PlacementDirection doorPlacement);
	// Gets closest door to given position
	Point2DInt GetClosestDoorPositionToPosition(Point2DInt position);
	// Add new door. Returns true, if operation was successful
	bool AddDoor(Point2DInt doorPos, Room* connectedTo);
	// Removes door from specific doorPlacement. Returns true, if operation was successful
	bool RemoveDoor(PlacementDirection doorPlacement);

	// Gets doors vector
	std::vector<Door> GetDoors() const { return _doors; }
	// Gets left top position
	Point2DInt GetLeftTopPosition() const { return _leftTopPosition; }
	// Gets right bottom position
	Point2DInt GetRightBottomPosition() const { return _rightBottomPosition; }
	// Get center of the room
	Point2DInt GetCenterOfTheRoomPosition() const {
		return Point2DInt((_leftTopPosition.x + _rightBottomPosition.x) / 2,
			(_leftTopPosition.y + _rightBottomPosition.y) / 2);
	}
	// Get size of the room (x - width, y - height)
	Point2DInt GetSizeOfRoom();
	// Checks, if position is inside of the room
	bool IsPositionInsideOfTheRoom(Point2DInt position) const{
		return position.y >= _leftTopPosition.y && position.y <= _rightBottomPosition.y
			&& position.x >= _leftTopPosition.x && position.x <= _rightBottomPosition.x;
	}
	// Does our room intersect with other room 
	bool DoesIntersectWithRoom(Room intersectRoom, int spaceBetweenRooms = 1);
	// Checks, if you can get to the given room, by using doors
	bool HasWayToRoom(Room& destinationRoom);
};

// Map class controls map cells and objects in it
// Holds rooms and information about them
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
		// Update position of each element
		for (int posY = 0; posY < mapHeight; posY++)
		{
			for (int posX = 0; posX < mapWidth; posX++)
			{
				_map[posY][posX].SetArrayPosition(Point2DInt(posX, posY));
			}
		}
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
	// Checks, inside which room position is
	// Returns nullptr, if position is not inside of any room
	Room* GetRoomByPosition(Point2DInt position);
	// Set room visibility and all objects inside of it
	void SetRoomVisibility(Room* room, bool isVisible);
	// Sets visibility for specific square on map
	void SetRectangleVisibility(Point2DInt leftTop, Point2DInt bottomRight, bool isVisible);
	// Get map width
	int GetMapWidth() const { return _mapWidth; }
	// Get map height
	int GetMapHeight() const { return _mapHeight; }	
	// Is point inside of the map
	bool IsPointInsideOfTheMap(Point2DInt position) const {
		return position.y >= 0 && position.y < GetMapHeight()
			&& position.x >= 0 && position.x < GetMapWidth();
	}

	// Function for moving, it checks if it possible to move on given position
	// Returns false - it is not possible to be reached or taken by another game object
	// Returns true - mapcell is reachable and free to go
	// MapCell object equals nullptr in cases, when it is not possible to reach position at all
	bool TryMoveCellObject(Point2DInt startPos, Point2DInt targetPoint, MapCell*& mapCellOnTargetPos);
};