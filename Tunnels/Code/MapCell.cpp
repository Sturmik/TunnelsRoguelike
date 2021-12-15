#include "MapCell.h"

void MapCell::SetCellState(CellState cellState)
{
	_cellState = cellState;
}

CellState MapCell::GetCellState()
{
	return _cellState;
}

GameObject* MapCell::GetGameObject()
{
	return _gameObject;
}
