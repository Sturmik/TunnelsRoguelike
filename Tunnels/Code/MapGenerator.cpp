#include "MapGenerator.h"

void MapGenerator::ClearMap()
{
	_map->GetRooms().clear();
	for (int posY = 0; posY < _map->GetMapHeight(); posY++)
	{
		for (int posX = 0; posX < _map->GetMapWidth(); posX++)
		{
			// Set cell state to none
			_map->GetMap2D()[posY][posX].SetCellState(CellState::None);
			// Set game object to nullptr
			_map->GetMap2D()[posY][posX].SetGameObject(nullptr);
			// Setting object to invisible state, so we won't draw unused parts of the map
			_map->GetMap2D()[posY][posX].SetMapCellVisibility(false);
		}
	}
}

bool MapGenerator::AddRoom(Room newRoom)
{
	// Check if we can fit the room
	if (!DoesRoomFitInMap(newRoom)) { return false; };
	// Add room
	_map->GetRooms().push_back(newRoom);
	// Draw this room
	// Getting position of room points
	Point2DInt topLeft = newRoom.GetLeftTopPosition();
	Point2DInt bottomRight = newRoom.GetRightBottomPosition();
	// Drawing room
	for (int posY = topLeft.y; posY <= bottomRight.y; posY++)
	{
		for (int posX = topLeft.x; posX <= bottomRight.x; posX++)
		{
			// Set cell state to free 
			_map->GetMap2D()[posY][posX].SetCellState(CellState::Free);
			// Set floor texture
			_map->GetMap2D()[posY][posX].SetTexture(_floorTexture);
			// Set position
			_map->GetMap2D()[posY][posX].setPosition
			(sf::Vector2f(posX + posX * _map->GetMap2D()[posY][posX].getScale().x,
				posY + posY * _map->GetMap2D()[posY][posX].getScale().y));
			// Draw walls
			if (posY == topLeft.y || posY == bottomRight.y
				|| posX == topLeft.x || posX == bottomRight.x)
			{
				// Set cell state to uncreachable
				_map->GetMap2D()[posY][posX].SetCellState(CellState::Unreachable);
				// Set wall texture
				_map->GetMap2D()[posY][posX].SetTexture(_wallTexture);
			}
		}
	}
	return true;
}

bool MapGenerator::CheckRoomForMapBoundaries(Room newRoom)
{
	// Check boundaries
	if (newRoom.GetLeftTopPosition().x >= _map->GetMapWidth() - MAP_BORDER_OFFSET ||
		newRoom.GetLeftTopPosition().y >= _map->GetMapHeight() - MAP_BORDER_OFFSET ||
		newRoom.GetLeftTopPosition().x < MAP_BORDER_OFFSET ||
		newRoom.GetLeftTopPosition().y < MAP_BORDER_OFFSET ||
		newRoom.GetRightBottomPosition().x >= _map->GetMapWidth() - MAP_BORDER_OFFSET ||
		newRoom.GetRightBottomPosition().y >= _map->GetMapHeight() - MAP_BORDER_OFFSET ||
		newRoom.GetRightBottomPosition().x < MAP_BORDER_OFFSET ||
		newRoom.GetRightBottomPosition().y < MAP_BORDER_OFFSET)
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
	for (int i = 0; i < _map->GetRooms().size(); i++)
	{
		// If our room collides 
		if (_map->GetRooms()[i].DoesIntersectWithRoom(newRoom, 2))
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
		int roomWidth = 3 + rand() % _map->GetMapWidth() / 4;
		int roomHeight = 3 + rand() % _map->GetMapHeight() / 4;
		// Random possible rooms
		std::vector<Room> possibleRooms;
		// Cycle for check if there any possible positions for this room
		while (true)
		{
			// Looking for positions
			for (int yPos = 0; yPos < _map->GetMapHeight(); yPos++)
			{
				for (int xPos = 0; xPos < _map->GetMapWidth(); xPos++)
				{
					// Generate position
					Point2DInt leftTop(xPos, yPos);
					Point2DInt rightBottom(xPos + roomWidth, yPos + roomHeight);
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

std::vector<Room*> MapGenerator::GetClosestRoomsToPosition(Point2DInt position, Room& roomToIgnore)
{
	// Form vector of closest rooms to the recent one
	std::map<double, Room*> roomsDistance;
	std::vector<double> roomsDistanceKeys;
	// Go through all rooms
	for (int roomAnalyze = 0; roomAnalyze < _map->GetRooms().size(); roomAnalyze++)
	{
		// Ignore specific room
		if (&roomToIgnore == &_map->GetRooms()[roomAnalyze]) { continue; }
		// Check distance
		double distance = position.Distance
		(_map->GetRooms()[roomAnalyze].GetClosestDoorPositionToPosition(position));
		roomsDistanceKeys.push_back(distance);
		roomsDistance[distance] = &_map->GetRooms()[roomAnalyze];
	}
	// Sort keys
	std::sort(roomsDistanceKeys.begin(), roomsDistanceKeys.end());
	// Form vector of rooms
	std::vector<Room*> closestRooms;
	for (int i = 0; i < roomsDistanceKeys.size(); i++)
	{
		closestRooms.push_back(roomsDistance[roomsDistanceKeys[i]]);
	}
	// Return formed vector
	return closestRooms;
}

void MapGenerator::GenerateTunnel(Room* startRoom, Point2DInt startPoint, Room* endRoom, Point2DInt endPoint)
{
	// Set cell states to free state
	_map->GetMap2D()[startPoint.y][startPoint.x].SetCellState(CellState::Free);
	_map->GetMap2D()[endPoint.y][endPoint.x].SetCellState(CellState::Free);

	// Form vector of passable cells and try to make the path
	std::vector<CellState> passableCells = { CellState::None, CellState::Free };
	std::list<Point2DInt> path = _pathFinding->GeneratePath(startPoint, endPoint, passableCells);
	// If path was not formed
	if (path.size() == 0)
	{
		// Mark cells as occupied again
		_map->GetMap2D()[startPoint.y][startPoint.x].SetCellState(CellState::Occupied);
		_map->GetMap2D()[endPoint.y][endPoint.x].SetCellState(CellState::Occupied);
		// Exit function
		return;
	}

	// Add doors, if needed
	startRoom->AddDoor(startPoint, endRoom);
	endRoom->AddDoor(endPoint, startRoom);

	// Form iterator
	std::list<Point2DInt>::iterator iter = path.begin(); iter++;
	std::list<Point2DInt>::iterator iEnd = path.end(); iEnd--;
	// Create tunnel
	for (iter; iter != iEnd; iter++)
	{
		// Set state
		_map->GetMap2D()[iter->y][iter->x].SetCellState(CellState::Free);
		// Set texture
		_map->GetMap2D()[iter->y][iter->x].SetTexture(_tunnelTexture);
		// Set position
		_map->GetMap2D()[iter->y][iter->x].setPosition
		(sf::Vector2f(iter->x + iter->x * _map->GetMap2D()[iter->y][iter->x].getScale().x,
			iter->y + iter->y * _map->GetMap2D()[iter->y][iter->x].getScale().y));
	}

	// Add textures on doors positions and update their state to occupied on start point
	_map->GetMap2D()[startPoint.y][startPoint.x].SetCellState(CellState::Occupied);
	_map->GetMap2D()[startPoint.y][startPoint.x].SetTexture(_doorTexture);

	// Add textures on doors positions and update their state to occupied on end point
	_map->GetMap2D()[endPoint.y][endPoint.x].SetCellState(CellState::Occupied);
	_map->GetMap2D()[endPoint.y][endPoint.x].SetTexture(_doorTexture);
}

void MapGenerator::GenerateRoomConnections()
{
	if (_map->GetRooms().size() <= 1) { return; }
	for (int room = 0; room < _map->GetRooms().size(); room++)
	{
		Room* roomConnector = &_map->GetRooms()[room];
		// Go through each potential door (top, right, bottom, left)
		for (int place = PlacementDirection::Top; place <= PlacementDirection::Left; place++)
		{
			// Door position
			Point2DInt doorPosition = roomConnector->GetDoorPositionOnPlacement(static_cast<PlacementDirection>(place));
			// Get closest rooms to our door position
			std::vector<Room*> closestRooms
				= GetClosestRoomsToPosition(doorPosition, *roomConnector);
			// Check, if there are any other rooms in given direction
			bool roomsExist = false;
			// Look for rooms
			for (int roomCheck = 0; roomCheck < closestRooms.size(); roomCheck++)
			{
				switch (place)
				{
				case Top:
					roomsExist = closestRooms[roomCheck]->GetClosestDoorPositionToPosition(doorPosition).y + MAP_BORDER_OFFSET < doorPosition.y;
					break;
				case Right:
					roomsExist = closestRooms[roomCheck]->GetClosestDoorPositionToPosition(doorPosition).x - MAP_BORDER_OFFSET > doorPosition.x;
					break;
				case Bottom:
					roomsExist = closestRooms[roomCheck]->GetClosestDoorPositionToPosition(doorPosition).y - MAP_BORDER_OFFSET > doorPosition.y;
					break;
				case Left:
					roomsExist = closestRooms[roomCheck]->GetClosestDoorPositionToPosition(doorPosition).x + MAP_BORDER_OFFSET < doorPosition.x;
					break;
				default:
					break;
				}
				if (!roomsExist) { closestRooms.erase(closestRooms.begin() + roomCheck); roomCheck -= 1; }
			}
			// Go through all closest rooms
			for (int closestRoomIt = 0; closestRoomIt < closestRooms.size(); closestRoomIt++)
			{
				// If, we can't get to that room, build the connection to it
				if (!roomConnector->HasWayToRoom(*closestRooms[closestRoomIt]))
				{
					// Generate tunnel with our recent door
					GenerateTunnel(roomConnector, doorPosition,
						closestRooms[closestRoomIt], closestRooms[closestRoomIt]->
						GetClosestDoorPositionToPosition(doorPosition));
					break;
				}
			}
		}
	}
}

void MapGenerator::GenerateMap(int numberOfRooms)
{
	// Clear all map
	ClearMap();
	// Generate rooms
	GenerateRooms(numberOfRooms);
	// Generate room doors and connections between them
	GenerateRoomConnections();
	// While, tunnels were generated, all doors were marked as occupied to 
	// prevent A* start algorithm to build tunnels inside of the rooms.
	// That's why, after the room connections generation was finished,
	// update all door cells states to "Free"
	for (int room = 0; room < _map->GetRooms().size(); room++)
	{
		// Set all doors to free state
		for (int door = 0; door < _map->GetRooms()[room].GetDoors().size(); door++)
		{
			// Get door position
			Point2DInt doorPos = _map->GetRooms()[room].GetDoors()[door].position;
			// Update cell state to free
			_map->GetMap2D()[doorPos.y][doorPos.x].SetCellState(CellState::Free);
		}
	}
}