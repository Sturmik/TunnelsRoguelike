#pragma once

#include "GameObject.h"

// Cell state
enum class CellState
{
	// Default state, used for marking cell, 
	// which are free for map generator to built on
	None,
	// Free for movement.
	Free,
	// Occupied, can't move here, without interaction with an object.
	Occupied,
	// Unreachable, means no interactive object can interact with it
	// nor move on it.
	Unreachable,
	// Invokable, specific state of the cell, which is used for trigger zones
	// which are used for various tasks. Example is ladder, which can be used 
	// by player, but not by enemies. So, in order for them to avoid this 
	// cell and don't try to interact with it, we mark it as Invokable.
	Invokable
};

class MapCell : public GameObject
{
private:
	// Position of map cell in array
	Point2DInt _mapCell2DPosition;
	// Cell state
	CellState _cellState;
	// Game object on cell
	GameObject* _gameObject;
public:
	// Constructors
	MapCell() : _gameObject(nullptr), _cellState(CellState::None){}
	MapCell(Point2DInt mapCell2DPosition, CellState cellState)
		: _mapCell2DPosition(mapCell2DPosition), _gameObject(nullptr), _cellState(cellState) {}

	// Sets map cell and object visibility 
	void SetMapCellVisibility(bool isVisible)
	{
		// Update map cell visibility
		SetObjectVisibility(isVisible);
		// If cell state is "None" don't update it's visibility
		if (_cellState == CellState::None) { return; }
		// Update game object visibility
		if (_gameObject != nullptr) { _gameObject->SetObjectVisibility(isVisible); }
	}

	// Sets map cell position in array
	void SetArrayPosition(Point2DInt mapCellPosition) { _mapCell2DPosition = mapCellPosition; }
	// Gets map cell position in array
	Point2DInt GetArrayPosition() const { return _mapCell2DPosition; }

	// Sets cell state
	void SetCellState(CellState cellState) { _cellState = cellState; }
	// Gets cell state
	CellState GetCellState() const { return _cellState; }

	void SetGameObject(GameObject* gameObject) { _gameObject = gameObject; }
	// Gets game object on cell
	GameObject* GetGameObject() {return _gameObject;}
};