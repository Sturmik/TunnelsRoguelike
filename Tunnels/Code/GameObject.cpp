#include "GameObject.h"

void GameObject::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	// If object is not active, don't draw it on screen
	if (!IsObjectActive()) { return; }
	// Apply the entity's transform 
	states.transform *= getTransform(); // GetTransform() is defined by sf::Transformable
	// Apply the texture
	states.texture = &_gameObjectTexture;
	// Draw the vertex array
	target.draw(_gameObjectVertices, states);
}

GameObject::GameObject()
{
	// Create a vertex array
	sf::VertexArray quad(sf::Quads, 4);
	// Define it as a rectangle
	quad[0].position = sf::Vector2f(1.f, 1.f);
	quad[1].position = sf::Vector2f(1.f, -1.f);
	quad[2].position = sf::Vector2f(-1.f, -1.f);
	quad[3].position = sf::Vector2f(-1.f, 1.f);
	// Set it to member variable
	_gameObjectVertices = quad;
	// Set object activity to true
	_isObjectActive = true;
	// Set default scale to the object
	setScale(DEFAULT_SCALE_SIZE, DEFAULT_SCALE_SIZE);
}

void GameObject::SetTexture(sf::Texture texture)
{ 
	_gameObjectTexture = texture;
	// Define texture area for gameobject
	_gameObjectVertices[3].texCoords = sf::Vector2f(0.f, 0.f);
	_gameObjectVertices[2].texCoords = sf::Vector2f(texture.getSize().x, 0.f);
	_gameObjectVertices[1].texCoords = sf::Vector2f(texture.getSize().x, texture.getSize().y);
	_gameObjectVertices[0].texCoords = sf::Vector2f(0.f, texture.getSize().y);
}