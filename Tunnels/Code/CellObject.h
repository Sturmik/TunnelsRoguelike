#pragma once

#include "Map.h"

 // Abstract class for all cell objects
 // Used for movement and setting position
class CellObject : public GameObject
{
private:
	// Sets new object position in map manager
	void SetNewPosition(Point2DInt newPos);
protected:
	// Map manager is used for object movement and navigation
	Map* _map;
	// Map cell in which holds the object
	MapCell* _mapCell;
	// Cell object constructor
	CellObject(Map* map, MapCell* mapCell) :
		_map(map), _mapCell(mapCell)
	{
		if (_mapCell == nullptr) { return; }
		// Update position
		SetNewPosition(mapCell->GetArrayPosition());
	}

	// Methods is called after position change,
	// Can be overriden for defining object behaviour
	virtual void OnPositionChange() {};
public:
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