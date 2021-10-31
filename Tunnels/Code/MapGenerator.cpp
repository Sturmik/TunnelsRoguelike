#include "MapGenerator.h"

#pragma region RoomMethods

Vector2Int Room::GetDoorPositionOnPlacement(Placement doorPlacement)
{
	for (int i = 0; i < _doors.size(); i++)
	{
		if (doorPlacement == _doors[i].placement)
		{
			return _doors[i].position;
		}
	}
	switch (doorPlacement)
	{
	case Placement::Top:
	case Placement::Bottom:
		return (Vector2Int(GetRightBottomPosition().x / 2,
			Placement::Top
			? GetLeftTopPosition().y
			: GetRightBottomPosition().y));
	case Placement::Right:
	case Placement::Left:
		return (Vector2Int(GetRightBottomPosition().y / 2,
			Placement::Top
			? GetLeftTopPosition().x
			: GetRightBottomPosition().x));
	default:
		break;
	}
	return Vector2Int(-1, -1);
}

bool Room::HasDoorOnPlacement(Placement doorPlacement)
{
	// Check placement, there can only be one door on one side
	for (int i = 0; i < _doors.size(); i++)
	{
		if (doorPlacement == _doors[i].placement)
		{
			return true;
		}
	}
	return false;
}

bool Room::AddDoor(Placement doorPlacementPos, int doorPos)
{
	// Check placement, there can only be one door on one side
	if (HasDoorOnPlacement(doorPlacementPos))
	{
		return false;
	}
	// Check coordinates
	Vector2Int doorCoordinates;
	switch (doorPlacementPos)
	{
	case Top:
	case Bottom:
		doorCoordinates.x = doorPos;
		if (doorPos <= _leftTopPosition.x || doorPos >= _rightBottomPosition.x)
		{
			doorCoordinates.x = (_rightBottomPosition.x + _leftTopPosition.x) / 2;
		}
		doorCoordinates.y = doorPlacementPos == Top ? _leftTopPosition.y : _rightBottomPosition.y;
		break;
	case Right:
	case Left:
		doorCoordinates.y = doorPos;
		if (doorPos <= _leftTopPosition.y || doorPos >= _rightBottomPosition.y)
		{
			doorCoordinates.y = (_rightBottomPosition.y + _leftTopPosition.y) / 2;
		}
		doorCoordinates.x = doorPlacementPos == Left ? _leftTopPosition.x : _rightBottomPosition.x;
		break;
	default:
		break;
	}
	// Add new door
	_doors.push_back(Door(doorCoordinates, doorPlacementPos));
	return true;
}

bool Room::RemoveDoor(Placement doorPlacement)
{
	for (int i = 0; i < _doors.size(); i++)
	{
		if (doorPlacement == _doors[i].placement)
		{
			// Erase door, if found
			_doors.erase(_doors.begin() + i);
			return true;
		}
	}
	return false;
}

void Room::Move(int dx, int dy)
{
	_leftTopPosition.x += dx; _leftTopPosition.y += dy;
	_rightBottomPosition.x += dx; _rightBottomPosition.y += dy;
	for (int i = 0; i < _doors.size(); i++)
	{
		_doors[i].position.x += dx;
		_doors[i].position.y += dy;
	}
}

bool Room::DoesIntersectWithRoom(Room intersectRoom, int spaceBetweenRooms)
{
	// Value of space between rooms must always be more or equal one
	if (spaceBetweenRooms < 1) { spaceBetweenRooms = 1; }
	// Getting intersection room info
	Vector2Int intersectRoomTopLeft = intersectRoom.GetLeftTopPosition();
	Vector2Int intersectRoomBottomRight = intersectRoom.GetRightBottomPosition();
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

Vector2Int Room::GetSizeOfRoom()
{
	return Vector2Int(_rightBottomPosition.x - _leftTopPosition.x,
		_rightBottomPosition.y - _leftTopPosition.y);
}

bool Room::HasWayToRoom(Room& room)
{
	// Go through each door in the room
	for (int i = 0; i < _doors.size(); i++)
	{
		// Check room by it's address
		if (_doors[i].connectedRoom == &room)
		{
			// If it is the room, we are looking for return true
			return true;
		}
	}
	// If there are no connections to the room, return false
	return false;
}

#pragma endregion

#pragma region MapGeneratorMethods

MapCell** MapGenerator::CreateArr2D(MapCell** array2D, int rows, int columns)
{
	array2D = new MapCell * [rows];
	for (int row = 0; row < rows; row++)
	{
		array2D[row] = new MapCell[columns];
	}
	return array2D;
}

void MapGenerator::DrawRoom(MapCell** map2D, Room& room)
{
	// Getting position of room points
	Vector2Int topLeft = room.GetLeftTopPosition();
	Vector2Int bottomRight = room.GetRightBottomPosition();
	// Loading texture for drawing
	sf::Texture floorText, doorText, wallText;
	floorText.loadFromFile("Textures\\CellOpen.png");
	doorText.loadFromFile("Textures\\CellDoor.png");
	wallText.loadFromFile("Textures\\CellWall.png");
	for (int posY = topLeft.y; posY <= bottomRight.y; posY++)
	{
		for (int posX = topLeft.x; posX <= bottomRight.x; posX++)
		{
			// Set texture
			map2D[posY][posX].SetTexture(floorText);
			// Set scale
			map2D[posY][posX].setScale
			(sf::Vector2f(map2D[posY][posX].getScale().x, map2D[posY][posX].getScale().y));
			// Set position
			map2D[posY][posX].setPosition
			(sf::Vector2f(posX + posX * map2D[posY][posX].getScale().x,
				posY +  posY * map2D[posY][posX].getScale().y));
			// Draw walls
			if (posY == topLeft.y || posY == bottomRight.y
				|| posX == topLeft.x || posX == bottomRight.x)
			{
				map2D[posY][posX].SetTexture(wallText);
				// Check, if door must be drawn
				for (int i = 0; i < room.GetDoors().size(); i++)
				{
					if (room.GetDoors()[i].position.x == posX &&
						room.GetDoors()[i].position.y == posY)
					{
						map2D[posY][posX].
							setScale
							(sf::Vector2f(map2D[posY][posX].getScale().x / 2,
								map2D[posY][posX].getScale().y / 2));
						map2D[posY][posX].SetTexture(doorText);
						break;
					}
				}
			}
		}
	}
}

bool MapGenerator::AddRoom(Room newRoom)
{
	// Check if we can fit the room
	if (!DoesRoomFitInMap(newRoom)) { return false; };
	// Add room
	_rooms.push_back(newRoom);
	DrawRoom(_map, _rooms[_rooms.size() - 1]);
	return true;
}

bool MapGenerator::CheckRoomForMapBoundaries(Room newRoom)
{
	// Check boundaries
	if (newRoom.GetLeftTopPosition().x >= _mapWidth ||
		newRoom.GetLeftTopPosition().y >= _mapHeight ||
		newRoom.GetLeftTopPosition().x < 0 ||
		newRoom.GetLeftTopPosition().y < 0 ||
		newRoom.GetRightBottomPosition().x >= _mapWidth ||
		newRoom.GetRightBottomPosition().y >= _mapHeight ||
		newRoom.GetRightBottomPosition().x < 0 ||
		newRoom.GetRightBottomPosition().y < 0)
	{
		return false;
	}
	return true;
}

bool MapGenerator::DoesRoomFitInMap(Room newRoom)
{
	// Check boundaries
	if (!CheckRoomForMapBoundaries(newRoom))
	{
		return false;
	}
	// Check intersection with other rooms
	for (int i = 0; i < _rooms.size(); i++)
	{
		// If our room collides 
		if (_rooms[i].DoesIntersectWithRoom(newRoom, 2))
		{
			return false;
		}
	}
	return true;
}

void MapGenerator::GenerateRooms(int numberOfRooms)
{
	while (numberOfRooms > 0)
	{
		// Generate room size
		int roomWidth = 3 + rand() % _mapWidth / 4;
		int roomHeight = 3 + rand() % _mapHeight / 4;
		// Random possible rooms
		std::vector<Room> possibleRooms;
		// Cycle for check if there any possible positions for this room
		while (true)
		{
			// Looking for positions
			for (int yPos = 0; yPos < _mapHeight; yPos++)
			{
				for (int xPos = 0; xPos < _mapWidth; xPos++)
				{
					// Generation position
					Vector2Int leftTop(xPos, yPos);
					Vector2Int rightBottom(xPos + roomWidth, yPos + roomHeight);
					// GenerateRoom
					Room newRoom(leftTop, rightBottom);
					// If room fits add it to positions
					if (DoesRoomFitInMap(newRoom)) { possibleRooms.push_back(newRoom); }
				}
			}
			// Exit cycle, if there are any rooms
			if (possibleRooms.size() > 0) { break; }
			// If there is no possible rooms, 
			// randomly decrease width or height and try again
			switch (rand() % 2)
			{
			case 0:
				if (roomWidth == 3)
				{
					if (roomHeight != 3)
					{
						roomHeight--;
						continue;
					}
					else
					{
						break;
					}
				}
				roomWidth--;
				continue;
			case 1:
				if (roomHeight == 3)
				{
					if (roomWidth != 3)
					{
						roomWidth--;
						continue;
					}
					else
					{
						break;
					}
				}
				roomHeight--;
				continue;
			default:
				return;
			}
			// If there are no rooms that fit even with minimal proportions
			// there is no any free space. End room generation.
			return;
		}
		// Add random possible room
		AddRoom(possibleRooms[rand() % possibleRooms.size()]);
		// Decrease number of rooms counter
		numberOfRooms--;
	}
}

std::vector<Room> MapGenerator::GetClosestRoomsToPosition(Vector2Int position, Room& roomToIgnore)
{
	// Form vector of closest rooms to the recent one
	std::map<double, Room> roomsDistance;
	std::vector<double> roomsDistanceKeys;
	// Go through all rooms
	for (int roomAnalyze = 0; roomAnalyze < _rooms.size(); roomAnalyze++)
	{
		// No point in checking our own room
		if (&roomToIgnore == &_rooms[roomAnalyze]) { continue; }
		// Check distance
		double distance = position.Distance
		(_rooms[roomAnalyze].GetCenterOfTheRoomPosition());
		roomsDistanceKeys.push_back(distance);
		roomsDistance[distance] = _rooms[roomAnalyze];
	}
	// Sort keys
	std::sort(roomsDistanceKeys.begin(), roomsDistanceKeys.end());
	// Form vector of rooms
	std::vector<Room> closestRooms;
	for (int i = 0; i < roomsDistanceKeys.size(); i++)
	{
		closestRooms.push_back(roomsDistance[roomsDistanceKeys[i]]);
	}
	// Return formed vector
	return closestRooms;
}

void MapGenerator::GenerateTunnel(Vector2Int startPoint, Vector2Int endPoint)
{

}

void MapGenerator::GenerateRoomConnections()
{
	// Go through each room and generate doors and connections
	// between other rooms, if possible
	for (int room = 0; room < _rooms.size(); room++)
	{
		// Placement for room doors generation
		Placement place = Placement::Top;
		// Go through each potential door (top, right, bottom, left)
		for (int place = Placement::Top; place <= Placement::Left; place++)
		{
			// Door position
			Vector2Int doorPosition = _rooms[room].GetDoorPositionOnPlacement((Placement)place);
			// Check, if there are any other rooms in given direction
			bool roomsExist = false;
			// Look for rooms
			for (int i = 0; i < _rooms.size(); i++)
			{
				if (room == i) continue;
				switch (place)
				{
				case Top:
					roomsExist = _rooms[room].GetCenterOfTheRoomPosition().y > doorPosition.y;
					break;
				case Right:
					roomsExist = _rooms[room].GetCenterOfTheRoomPosition().x > doorPosition.x;
					break;
				case Bottom:
					roomsExist = _rooms[room].GetCenterOfTheRoomPosition().y < doorPosition.y;
					break;
				case Left:
					roomsExist = _rooms[room].GetCenterOfTheRoomPosition().x < doorPosition.x;
					break;
				default:
					break;
				}
				if (roomsExist) { break; }
			}
			// If rooms exist initiate search
			if (roomsExist)
			{
				// Get closest rooms to our door position
				std::vector<Room> closestRooms = GetClosestRoomsToPosition(doorPosition, _rooms[room]);
				// Go through all closest rooms
				for (int i = 0; i < closestRooms.size(); i++)
				{
					// If, we can't get to that room, build the connection to it
					if (!_rooms[room].HasWayToRoom(closestRooms[i]))
					{

					}
				}
			}
		}
	}
}

#pragma endregion 