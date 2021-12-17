#include "Map.h"

Point2DInt Room::GetDoorPositionOnPlacement(PlacementDirection doorPlacement)
{
	// If door exists, get it position
	for (int i = 0; i < _doors.size(); i++)
	{
		if (doorPlacement == _doors[i].doorPlacement)
		{
			return _doors[i].position;
		}
	}
	// Else, get approximate central position of the door
	Point2DInt result;
	switch (doorPlacement)
	{
	case PlacementDirection::Top:
	case PlacementDirection::Bottom:
		result = Point2DInt((_leftTopPosition.x + _rightBottomPosition.x) / 2,
			doorPlacement == PlacementDirection::Top
			? _leftTopPosition.y
			: _rightBottomPosition.y);
		break;
	case PlacementDirection::Right:
	case PlacementDirection::Left:
		result = Point2DInt(doorPlacement == PlacementDirection::Left
			? _leftTopPosition.x
			: _rightBottomPosition.x,
			(_leftTopPosition.y + _rightBottomPosition.y) / 2);
		break;
	default:
		break;
	}
	return result;
}

bool Room::HasDoorOnPlacement(PlacementDirection doorPlacement)
{
	// Check doorPlacement, there can only be one door on one side
	for (int i = 0; i < _doors.size(); i++)
	{
		if (doorPlacement == _doors[i].doorPlacement)
		{
			return true;
		}
	}
	return false;
}

Point2DInt Room::GetClosestDoorPositionToPosition(Point2DInt position)
{
	Point2DInt closestPosition;
	double minDistance;
	for (int placement = PlacementDirection::Top; placement <= PlacementDirection::Left; placement++)
	{
		// Getting closest door position
		Point2DInt doorPos = GetDoorPositionOnPlacement(static_cast<PlacementDirection>(placement));
		// Calculating distance to it
		double distance = doorPos.Distance(position);
		// Checking if it is closer than previous
		if (placement == PlacementDirection::Top || distance < minDistance)
		{
			minDistance = distance; closestPosition = doorPos;
		}
	}
	return closestPosition;
}

bool Room::AddDoor(Point2DInt doorPos, Room* connectedTo)
{
	int previousNumOfDoors = _doors.size();
	// Checking left and right wall 
	if (doorPos.y >= _leftTopPosition.y && doorPos.y <= _rightBottomPosition.y)
	{
		// Left door
		if (doorPos.x == _leftTopPosition.x)
		{
			if (!HasDoorOnPlacement(PlacementDirection::Left)) {
				_doors.push_back(Door(doorPos, PlacementDirection::Left));
			}
		}
		// Right door
		if (doorPos.x == _rightBottomPosition.x)
		{
			if (!HasDoorOnPlacement(PlacementDirection::Right)) {
				_doors.push_back(Door(doorPos, PlacementDirection::Right));
			}
		}
	}
	// Checking top and bottom wall
	if (doorPos.x >= _leftTopPosition.x && doorPos.x <= _rightBottomPosition.x)
	{
		// Top wall
		if (doorPos.y == _leftTopPosition.y)
		{
			if (!HasDoorOnPlacement(PlacementDirection::Top)) {
				_doors.push_back(Door(doorPos, PlacementDirection::Top));
			}
		}
		// Bottom wall
		if (doorPos.y == _rightBottomPosition.y)
		{
			if (!HasDoorOnPlacement(PlacementDirection::Bottom)) {
				_doors.push_back(Door(doorPos, PlacementDirection::Bottom));
			}
		}
	}
	// Is door created
	bool isDoorCreated = _doors.size() > previousNumOfDoors;
	if (isDoorCreated)
	{
		_doors[_doors.size() - 1].connectedRoom = connectedTo;
	}
	return isDoorCreated;
}

bool Room::RemoveDoor(PlacementDirection doorPlacement)
{
	for (int i = 0; i < _doors.size(); i++)
	{
		if (doorPlacement == _doors[i].doorPlacement)
		{
			// Erase door, if found
			_doors.erase(_doors.begin() + i);
			return true;
		}
	}
	return false;
}

bool Room::DoesIntersectWithRoom(Room intersectRoom, int spaceBetweenRooms)
{
	// Value of space between rooms must always be more or equal one
	if (spaceBetweenRooms < 1) { spaceBetweenRooms = 1; }
	// Getting intersection room info
	Point2DInt intersectRoomTopLeft = intersectRoom.GetLeftTopPosition();
	Point2DInt intersectRoomBottomRight = intersectRoom.GetRightBottomPosition();
	// Checking intersection
	for (int pointX = _leftTopPosition.x; pointX <= _rightBottomPosition.x; pointX++)
	{
		for (int pointY = _leftTopPosition.y; pointY <= _rightBottomPosition.y; pointY++)
		{
			if (pointX >= intersectRoomTopLeft.x - spaceBetweenRooms &&
				pointX <= intersectRoomBottomRight.x + spaceBetweenRooms &&
				pointY >= intersectRoomTopLeft.y - spaceBetweenRooms &&
				pointY <= intersectRoomBottomRight.y + spaceBetweenRooms)
			{
				return true;
			}
		}
	}
	return false;
}

Point2DInt Room::GetSizeOfRoom()
{
	return Point2DInt(_rightBottomPosition.x - _leftTopPosition.x,
		_rightBottomPosition.y - _leftTopPosition.y);
}

bool Room::HasWayToRoom(Room& destinationRoom, std::vector<Room*>& visitedRooms)
{
	// Return true, if we are at the destination room
	if (&destinationRoom == this) return true;
	// Add recent room to visited ones
	visitedRooms.push_back(this);
	// Boolean variable for room searching
	bool hasWay = true; bool isVisited;
	// Go through each door in the room
	for (int i = 0; i < _doors.size(); i++)
	{
		// Check, if the room to which the door lead
		// was already visited
		isVisited = false;
		for (int roomCheck = 0; roomCheck < visitedRooms.size(); roomCheck++)
		{
			if (visitedRooms[roomCheck] == _doors[i].connectedRoom)
			{
				isVisited = true;
				break;
			}
		}
		if (isVisited) { continue; }
		// Check room by it's address
		if (_doors[i].connectedRoom == &destinationRoom)
		{
			// If it is the room, we are looking for return true
			return true;
		}
		hasWay = _doors[i].connectedRoom->HasWayToRoom(destinationRoom, visitedRooms);
		if (hasWay) return true;
	}
	// If there are no connections to the room, return false
	return false;
}

bool Room::HasWayToRoom(Room& destinationRoom)
{
	std::vector<Room*> visitedRooms;
	return HasWayToRoom(destinationRoom, visitedRooms);
}

Room* Map::GetRoomByPosition(Point2DInt position)
{
	// Iterate through each room
	for (std::vector<Room>::iterator it = _rooms.begin(); it != _rooms.end(); it++)
	{
		// If position is inside of the room - return this room
		if (it->IsPositionInsideOfTheRoom(position)) { return &(*it); }
	}
	return nullptr;
}

void Map::SetRoomVisibility(Room* room, bool isVisible)
{
	if (room == nullptr) { return; }
	// Make all room visible
	SetRectangleVisibility(room->GetLeftTopPosition(), room->GetRightBottomPosition(), isVisible);
}

void Map::SetRectangleVisibility(Point2DInt leftTop, Point2DInt bottomRight, bool isVisible)
{
	// Set visibility for square
	for (int y = leftTop.y; y <= bottomRight.y; y++)
	{
		for (int x = leftTop.x; x <= bottomRight.x; x++)
		{
			// Set visibility of map cell to true, if point is inside the map
			if (IsPointInsideOfTheMap(Point2DInt(x, y)))
			{
				// If cell is not marked as none - updates it's visibility
				if (_map[y][x].GetCellState() != CellState::None)
				{
					_map[y][x].SetMapCellVisibility(isVisible);
				}
			}
		}
	}
}

bool Map::TryMoveCellObject(Point2DInt startPos, Point2DInt targetPoint, MapCell*& mapCellOnTargetPos)
{
	// Set map cell object to null
	mapCellOnTargetPos = nullptr;
	// Check, if object tries to get out of map boundaries
	if (!IsPointInsideOfTheMap(targetPoint))
	{
		// Return false
		return false;
	}
	// Update map cell object
	mapCellOnTargetPos = &GetMap2D()[targetPoint.y][targetPoint.x];
	// Check, if there it possible to get to that position
	return mapCellOnTargetPos->GetCellState() == CellState::Free;
}