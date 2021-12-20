#include "CellObject.h"

void CellObject::SetNewPosition(Point2DInt newPos)
{
	// If before position change method returns false - don't move cell object
	if (!BeforePositionChange()) { return; }
	// New position
	if (_mapCell == nullptr)
	{
		// Set object in cell by default to occupied
		_map->GetMap2D()[newPos.y][newPos.x].SetCellState(CellState::Occupied);
		_map->GetMap2D()[newPos.y][newPos.x].SetGameObject(this);
	}
	// Change of position
	else
	{
		CellState pastCellState = _map->GetMap2D()[newPos.y][newPos.x].GetCellState();
		// MoveCellObject to new position
		_map->GetMap2D()[newPos.y][newPos.x].SetCellState(_mapCell->GetCellState());
		_map->GetMap2D()[newPos.y][newPos.x].SetGameObject(this);
		// Free old position
		_mapCell->SetCellState(CellState::Free);
		_mapCell->SetGameObject(nullptr);
	}
	// Update map cell
	_mapCell = &_map->GetMap2D()[newPos.y][newPos.x];
	// Update object position
	setPosition(_mapCell->getPosition());
	// Make object visible
	SetObjectVisibility(_mapCell->IsObjectVisible());
	// Call on position change method for specific changes
	OnPositionChange();
}

bool CellObject::MoveCellObject(PlacementDirection moveDirection, MapCell*& targetMapCell)
{
	if (_mapCell == nullptr) { return false; }
	// Define the way, we need to move
	switch (moveDirection)
	{
	case PlacementDirection::Top:
		return SetCellObjectPosition(Point2DInt(_mapCell->GetArrayPosition().x,
				_mapCell->GetArrayPosition().y - 1), targetMapCell);
			break;
	case PlacementDirection::Right:
		return SetCellObjectPosition(Point2DInt(_mapCell->GetArrayPosition().x + 1,
			_mapCell->GetArrayPosition().y), targetMapCell);
		break;
	case PlacementDirection::Bottom:
		return SetCellObjectPosition(Point2DInt(_mapCell->GetArrayPosition().x,
			_mapCell->GetArrayPosition().y + 1), targetMapCell);
			break;
	case PlacementDirection::Left:
		return SetCellObjectPosition(Point2DInt(_mapCell->GetArrayPosition().x - 1,
			_mapCell->GetArrayPosition().y), targetMapCell);
		break;
	default:
		return false;
		break;
	}
}

bool CellObject::SetCellObjectPosition(Point2DInt position, MapCell*& targetMapCell)
{
	if (_mapCell == nullptr) { SetNewPosition(position); return true; }
	// If place is free to rellocate, then move on it
	if (_map->TryMoveCellObject(_mapCell->GetArrayPosition(), position, targetMapCell) == true)
	{
		SetNewPosition(position);
		return true;
	}
	// Else, it is impossible to move there
	else
	{
		return false;
	}
}

bool CellObject::SetCellObjectPosition(Point2DInt position)
{
	MapCell* mapCell;
	if (_mapCell == nullptr) { SetNewPosition(position); return true; }
	// If place is free to rellocate, then move on it
	if (_map->TryMoveCellObject(_mapCell->GetArrayPosition(), position, mapCell) == true)
	{
		SetNewPosition(position);
		return true;
	}
	// Else, it is impossible to move there
	else
	{
		return false;
	}
}

void CellObject::RemoveFromMapCell()
{
	// If map cell is nullptr - return
	if (_mapCell == nullptr) { return; }
	// If object is on the map cell - set it to free state,
	// otherwise don't change anything
	_mapCell->SetCellState(_mapCell->GetGameObject() == this
		? CellState::Free : _mapCell->GetCellState());
	// If object is on the map cell - set it's object pointer to zero,
	// otherwise don't change anything
	_mapCell->SetGameObject(_mapCell->GetGameObject() == this
		? nullptr : _mapCell->GetGameObject());
	// Set assigned map cell to zero
	_mapCell = nullptr;
	// Set object to invisible state
	SetObjectVisibility(false);
}