#pragma once

#include "GameObject.h"

// Cell state
enum class CellState
{
	Free,
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
	MapCell() : MapCell(CellState::Free, nullptr) { }
	MapCell(CellState cellState, GameObject* objectOnCell) 
		: _cellState(cellState), _gameObject(objectOnCell) {}

	// Sets cell state
	void SetCellState(CellState cellState);
	// Gets cell state
	CellState GetCellState();

	// Sets game object on cell
	void SetGameObject(GameObject* gameObject);
	// Gets game object on cell
	GameObject* GetGameObject();
#pragma endregion
};