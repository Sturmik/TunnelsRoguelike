#include "MapGenerator.h"

#pragma region RoomMethods

Point2DInt Room::GetDoorPositionOnPlacement(DoorPlacement doorPlacement)
{
	for (int i = 0; i < _doors.size(); i++)
	{
		if (doorPlacement == _doors[i].placement)
		{
			return _doors[i].position;
		}
	}
	Point2DInt result;
	switch (doorPlacement)
	{
	case DoorPlacement::Top:
	case DoorPlacement::Bottom:
		result = Point2DInt((_leftTopPosition.x + _rightBottomPosition.x) / 2,
			doorPlacement == DoorPlacement::Top
			? _leftTopPosition.y
			: _rightBottomPosition.y);
		break;
	case DoorPlacement::Right:
	case DoorPlacement::Left:
		result = Point2DInt(doorPlacement == DoorPlacement::Left
			? _leftTopPosition.x
			: _rightBottomPosition.x,
			(_leftTopPosition.y + _rightBottomPosition.y) / 2);
		break;
	default:
		break;
	}
	return result;
}

bool Room::HasDoorOnPlacement(DoorPlacement doorPlacement)
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

Point2DInt Room::GetClosestDoorPositionToPosition(Point2DInt position)
{
	Point2DInt closestPosition;
	double minDistance;
	for (int i = DoorPlacement::Top; i <= DoorPlacement::Left; i++)
	{
		Point2DInt doorPos = GetDoorPositionOnPlacement(static_cast<DoorPlacement>(i));
		double distance = doorPos.Distance(position);
		if (i == DoorPlacement::Top || distance < minDistance)
		{ minDistance = distance; closestPosition = doorPos; }
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
			if (!HasDoorOnPlacement(DoorPlacement::Left)) {
				_doors.push_back(Door(doorPos, DoorPlacement::Left));
			}
		}
		// Right door
		if (doorPos.x == _rightBottomPosition.x)
		{
			if (!HasDoorOnPlacement(DoorPlacement::Right)) {
				_doors.push_back(Door(doorPos, DoorPlacement::Right));
			}
		}
	}
	// Checking top and bottom wall
	if (doorPos.x >= _leftTopPosition.x && doorPos.x <= _rightBottomPosition.x)
	{
		// Top wall
		if (doorPos.y == _leftTopPosition.y)
		{
			if (!HasDoorOnPlacement(DoorPlacement::Top)) {
				_doors.push_back(Door(doorPos, DoorPlacement::Top));
			}
		}
		// Bottom wall
		if (doorPos.y == _rightBottomPosition.y)
		{
			if (!HasDoorOnPlacement(DoorPlacement::Bottom)) {
				_doors.push_back(Door(doorPos, DoorPlacement::Bottom));
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

bool Room::RemoveDoor(DoorPlacement doorPlacement)
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

#pragma endregion

#pragma region ANavigation

bool PathFindingAStar::IsCellPassable(std::vector<CellState>& passableCells, Point2DInt cellPos)
{
	bool isPassable = false;
	for (int i = 0; i < passableCells.size(); i++)
	{
		if (_map->GetMap2D()[cellPos.y][cellPos.x].GetCellState() == passableCells[i])
		{
			isPassable = true;
			break;
		}
	}
	return isPassable;
}

std::vector<Point2DInt> PathFindingAStar::GeneratePath(Point2DInt startPoint, Point2DInt endPoint,
	std::vector<CellState>& passableCells)
{
	// Check, if start or end position are passable cells
	if (!IsCellPassable(passableCells, startPoint)
		|| !IsCellPassable(passableCells, endPoint))
	{
		return std::vector<Point2DInt>();
	}

	// Form node map
	FormNodeMap(startPoint, endPoint, passableCells);

	// Unchecked nodes list
	std::list<ANode*> uncheckedNodesList;
	uncheckedNodesList.push_back(&_aNodeMap[startPoint.y][startPoint.x]);

	// bool check, if we got to the point
	bool hasFoundEndPoint = false;

	// While there are nodes to check
	while (!uncheckedNodesList.empty())
	{
		// Sort nodes by global goal, so lowest is firts
		uncheckedNodesList.sort([](const ANode* lNode, const ANode* rNode) {return lNode->GetGlobalDistance() < rNode->GetGlobalDistance(); });

		// If unchecked node is open or an obstacle, discard it
		while (uncheckedNodesList.front()->IsOpen() ||
			uncheckedNodesList.front()->IsObstacle())
		{
			uncheckedNodesList.pop_front();
			// If list is empty, that means there is no way,
			// we can get to our position. Return empty vector
			if (uncheckedNodesList.empty()) { return std::vector<Point2DInt>(); }
		}
		// Get node
		ANode* aNode = uncheckedNodesList.front();
		// Set node to opened state
		aNode->SetOpenState(true);
		// Pop it out from nodes
		uncheckedNodesList.pop_front();
		// Go through all its neighbours
		for (int i = 0; i < aNode->GetNeighbours().size(); i++)
		{
			// Node is not an obstacle or is not opened
			if (!aNode->GetNeighbours()[i]->IsObstacle() ||
				!aNode->GetNeighbours()[i]->IsOpen())
			{
				// Add to unchecked nodes
				uncheckedNodesList.push_back(aNode->GetNeighbours()[i]);

				// If local distance is less than neighbour node
				if (aNode->GetLocalDistance() + 1 < aNode->GetNeighbours()[i]->GetLocalDistance())
				{
					// Set new parent
					aNode->GetNeighbours()[i]->SetParent(aNode);
					// Set new local distance
					aNode->GetNeighbours()[i]->SetLocalDistance(aNode->GetLocalDistance() + 1);
					// Check, if we are at the end point
					if (aNode->GetNeighbours()[i]->GetDistanceToTheEndPoint() == 0)
					{
						hasFoundEndPoint = true;
						break;
					}
				}
			}
		}

		// If we got to the end point - leave
		if (hasFoundEndPoint) { break; }
	}

	// Path
	std::vector<Point2DInt> path;
	// Fill path with data, if we got to the end point
	if (hasFoundEndPoint)
	{
		// Node for path generating
		ANode* nodeForPathGenerating = &_aNodeMap[endPoint.y][endPoint.x];
		// Until we hit start node
		while (nodeForPathGenerating->GetParent() != nullptr)
		{
			// Push position of node
			path.push_back(nodeForPathGenerating->GetPosition());
			// Change node to parent
			nodeForPathGenerating = nodeForPathGenerating->GetParent();
		}
		path.push_back(nodeForPathGenerating->GetPosition());
	}
	// Return path
	return path;
}

#pragma endregion


#pragma region MapGeneratorMethods

void MapGenerator::ClearMap()
{
	// Sets all to default
	sf::Texture empty;
	_map->GetRooms().clear();
	for (int posY = 0; posY < _map->GetMapHeight(); posY++)
	{
		for (int posX = 0; posX < _map->GetMapWidth(); posX++)
		{
			_map->GetMap2D()[posY][posX].SetCellState(CellState::None);
			_map->GetMap2D()[posY][posX].SetTexture(empty);
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
				// Set cell state to occupied
				_map->GetMap2D()[posY][posX].SetCellState(CellState::Occupied);
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
	std::vector<Point2DInt> path = _pathFinding->GeneratePath(startPoint, endPoint, passableCells);
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

	// Create tunnel
	for (int i = 1; i < path.size() - 1; i++)
	{
		// Set state
		_map->GetMap2D()[path[i].y][path[i].x].SetCellState(CellState::Free);
		// Set texture
		_map->GetMap2D()[path[i].y][path[i].x].SetTexture(_tunnelTexture);
		// Set position
		_map->GetMap2D()[path[i].y][path[i].x].setPosition
		(sf::Vector2f(path[i].x + path[i].x * _map->GetMap2D()[path[i].y][path[i].x].getScale().x,
			path[i].y + path[i].y * _map->GetMap2D()[path[i].y][path[i].x].getScale().y));
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
	for (int room = 0; room < _map->GetRooms().size(); room++)
	{
		Room* roomConnector = &_map->GetRooms()[room];
		// Go through each potential door (top, right, bottom, left)
		for (int place = DoorPlacement::Top; place <= DoorPlacement::Left; place++)
		{
			// Door position
			Point2DInt doorPosition = roomConnector->GetDoorPositionOnPlacement(static_cast<DoorPlacement>(place));
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

#pragma endregion 