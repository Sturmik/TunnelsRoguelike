#include "GameObject.h"

void GameObject::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
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
	// Define its texture area
	quad[0].texCoords = sf::Vector2f(1.f, 1.f);
	quad[1].texCoords = sf::Vector2f(1.f, -1.f);
	quad[2].texCoords = sf::Vector2f(-1.f, -1.f);
	quad[3].texCoords = sf::Vector2f(-1.f, 1.f);
	// Set it to member variable
	_gameObjectVertices = quad;
}

void GameObject::SetTexture(sf::Texture texture)
{ 
	_gameObjectTexture = texture;
}

void GameObject::SetVectorArray(sf::VertexArray vertexArray) 
{ 
	_gameObjectVertices = vertexArray;
}