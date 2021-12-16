#pragma once

#include "MapGenerator.h"

 // Basic class for all cell objects
 // Used for movement and setting position
class CellObject : public GameObject
{
protected:
	// Map manager is used for object movement and navigation
	Map* _map;
	// Map cell in which holds the object
	MapCell* _mapCell;

	// Sets new object position in map manager
	void SetNewPosition(Point2DInt newPos);
public:
	// Cell object constructor
	CellObject(Map* map, MapCell* mapCell) :
		_map(map), _mapCell(mapCell)
	{
		if (_mapCell == nullptr) { return; }
		// Set position of the object to its map cell
		_mapCell->setPosition(_mapCell->getPosition());
		// Update map cell game object
		_mapCell->SetGameObject(this);
		// By default cell object sets cell state to occupied
		_mapCell->SetCellState(CellState::Occupied);
	}

	// Deconstructor
	~CellObject()
	{
		RemoveFromMapCell();
	}

	// MoveCellObject cell object
	// Returns true - change of position is successful
	// Returns false - impossible to change position, check target map cell
	bool MoveCellObject(PlacementDirection moveDirection, MapCell*& targetMapCell);
	// Set cell object position
	// Returns true - change of position is successful
	// Returns false - impossible to change position, check target map cell
	bool SetCellObjectPosition(Point2DInt position, MapCell*& targetMapCell);
	// Set cell object position
	// Returns true - change of position is successful
	// Returns false - impossible to change position
	bool SetCellObjectPosition(Point2DInt position);
	// Removes object from recent map cell
	virtual void RemoveFromMapCell();
};