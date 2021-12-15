#pragma once

#include "GameObject.h"

// Cell state
enum class CellState
{
	// Default state
	None,
	// Free for movement
	Free,
	// Occupied, can't move here, without interaction with object
	Occupied
};

class MapCell : public GameObject
{
private:
	// Cell state
	CellState _cellState;
	// Game object on cell
	GameObject* _gameObject;
public:
	// Constructors
	MapCell() : MapCell(CellState::None) { }
	MapCell(CellState cellState) 
		: _cellState(cellState) {}

	// Sets cell state
	void SetCellState(CellState cellState);
	// Gets cell state
	CellState GetCellState();

	void SetGameObject(GameObject* gameObject) { _gameObject = gameObject; }
	// Gets game object on cell
	GameObject* GetGameObject();
#pragma endregion
};