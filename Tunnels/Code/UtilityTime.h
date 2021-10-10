#include <SFML/Graphics.hpp>

// Utility time static class 
// - Allows to get delta time
static class UtilityTime
{
private:
#pragma region Variables
    static sf::Time _setTime;
    static sf::Clock _deltaClock;
#pragma endregion
#pragma region Constructor
    // It is singleton, so we won't allow to initialize it
    UtilityTime();
#pragma endregion
public:
#pragma region Methods
    // Start frame timer (restarts timer)
    static void FrameStartTime();
    // Gets delta time as sf::Time
    static sf::Time GetDeltaTimeAsSfTime();
    // Gets deltat time as float value
    static float GetDeltaTime();
#pragma endregion
};