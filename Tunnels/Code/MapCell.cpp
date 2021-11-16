#include "MapCell.h"

void MapCell::SetCellState(CellState cellState)
{
	_cellState = cellState;
}

CellState MapCell::GetCellState()
{
	return _cellState;
}

std::vector<GameObject*>& MapCell::GetGameObjectVector()
{
	return _gameObject;
}
