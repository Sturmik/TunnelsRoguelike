#include "MapCell.h"

void MapCell::SetCellState(CellState cellState)
{
	_cellState = cellState;
}

CellState MapCell::GetCellState()
{
	return _cellState;
}

void MapCell::SetGameObject(GameObject* gameObject)
{
	_gameObject = gameObject;
}

GameObject* MapCell::GetGameObject()
{
	return _gameObject;
}
