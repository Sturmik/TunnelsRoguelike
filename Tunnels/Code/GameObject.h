#pragma once

#include <SFML/Graphics.hpp>

// Point struct, represents x and y
struct Point2DInt
{
	int x, y;
	Point2DInt() { x = 0; y = 0; }
	Point2DInt(int x, int y) : x(x), y(y) {}

	// Calculates distance between recent and given point
	double Distance(Point2DInt vector)
	{
		return (x - vector.x) * (x - vector.x) + (y - vector.y) * (y - vector.y);
	}
};

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