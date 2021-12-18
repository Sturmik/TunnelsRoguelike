#pragma once

#include "Map.h"

#include <limits.h>

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

		Point2DInt GetArrayPosition() const { return _position; }
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

		void SetArrayPosition(Point2DInt position) { _position = position; }
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
		std::vector<CellState>& passableCells);
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
	std::list<Point2DInt> GeneratePath(Point2DInt startPoint, Point2DInt endPoint,
		std::vector<CellState> passableCells);
};