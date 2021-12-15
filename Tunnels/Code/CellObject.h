#include "MapManager.h"

// Basic class for all cell objects
// Is used for their movement and setting position
//class CellObject : GameObject
//{
//private:
//	// Map manager is used for object movement and navigation
//	MapManager* _mapManager;
//	// Cell object position
//	Point2DInt _cellObjectPosition;
//public:
//	// Cell object constructor
//	CellObject(MapManager* mapManager, Point2DInt cellObjectPosition) : _mapManager(mapManager), _cellObjectPosition(cellObjectPosition) {}
//
//	// Object moving
//	bool Move(PlacementDirection moveDirection);
//	// Object position set
//	bool SetPosition(Point2DInt position)
//};