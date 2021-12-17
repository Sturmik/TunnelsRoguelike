#include "PathFindingAStar.h"

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

void PathFindingAStar::FormNodeMap(Point2DInt startPoint, Point2DInt endPoint, std::vector<CellState>& passableCells)
{
	// Edit every node according to given data
	for (int row = 0; row < _map->GetMapHeight(); row++)
	{
		for (int col = 0; col < _map->GetMapWidth(); col++)
		{
			// Set node to default values
			_aNodeMap[row][col].SetNodeToDefaultValues();
			// Setting position
			_aNodeMap[row][col].SetArrayPosition(Point2DInt(col, row));
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
			path.push_back(nodeForPathGenerating->GetArrayPosition());
			// Change node to parent
			nodeForPathGenerating = nodeForPathGenerating->GetParent();
		}
		path.push_back(nodeForPathGenerating->GetArrayPosition());
	}
	// Return path
	return path;
}