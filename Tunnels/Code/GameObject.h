#pragma once

#include <SFML/Graphics.hpp>

// Base Game object class for all active objects in game
class GameObject : public sf::Drawable,  public sf::Transformable
{
private:
	// Default size for all game objects
	const int DEFAULT_SCALE_SIZE = 15;

	// Vertex array
	sf::VertexArray _gameObjectVertices;
	// Game object texture
	sf::Texture _gameObjectTexture;
	// Is object active, will it be drawn on the screen
	bool _isObjectActive;

	// Allows to draw your own entinity
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
public:
	// Default constructor
	GameObject();

	// Return object activity state
	bool IsObjectActive() const { return _isObjectActive; }
	// Sets object activity
	void SetObjectActivity(bool activity) { _isObjectActive = activity; }
	// Set texture for gameobject
	void SetTexture(sf::Texture texture);
};