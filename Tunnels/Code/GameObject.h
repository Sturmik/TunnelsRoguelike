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
	bool _isObjectVisible;
	// If object is dead, it will be deleted in next cycle of the programm
	bool _isObjectDead;

	// Allows to draw your own entinity
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
public:
	// Default constructor
	GameObject();

	// Updates game object 
	virtual void Update() {}
	// Return object visibility state
	bool IsObjectVisible() const { return _isObjectVisible; }
	// Sets object visibility
	void SetObjectVisibility(bool isVisible) { _isObjectVisible = isVisible; }
	// Return object state
	bool IsObjectDead() const { return _isObjectDead; }
	// Sets object state
	void SetObjectDeathState(bool isDead) { _isObjectDead = isDead; }
	// Set texture for gameobject
	void SetTexture(sf::Texture texture);
	// Sets texture from path
	void SetTexture(std::string texturePath);
};