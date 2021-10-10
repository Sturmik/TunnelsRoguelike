#include <SFML/Graphics.hpp>

// Base Game object class for all active objects in game
class GameObject : public sf::Drawable,  public sf::Transformable
{
private:
#pragma region Variables
	// Vertex array
	sf::VertexArray _gameObjectVertices;
	// Game object texture
	sf::Texture _gameObjectTexture;
#pragma endregion
#pragma region Methods
	// Allows to draw your own entinity
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
#pragma endregion
public:
#pragma region Constructor
	// Default constructor
	GameObject();
#pragma endregion
#pragma region Methods
	// Set texture for gameobject
	void SetTexture(sf::Texture texture);
#pragma endregion
};