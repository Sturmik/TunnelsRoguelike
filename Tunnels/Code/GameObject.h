#include <SFML/Graphics.hpp>

// Base Game object class for all active objects in game
class GameObject : public sf::Drawable,  public sf::Transformable
{
private:
	sf::VertexArray _gameObjectVertices;
	sf::Texture _gameObjectTexture;

	// Allows to draw your own entinity
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
public:
	// Default constructor
	GameObject();
	// Set texture for gameobject
	void SetTexture(sf::Texture texture);
	// Set vector array of game object
	void SetVectorArray(sf::VertexArray vertexArray);
};