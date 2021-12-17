#pragma once

#include <SFML/Graphics.hpp>

// Utility time static class 
// - Allows to get delta time
static class UtilityTime
{
private:
    static sf::Time _setTime;
    static sf::Clock _deltaClock;

    // It is functor class, so we won't allow to initialize it
    UtilityTime();
public:
    // Start frame timer (restarts timer)
    static void FrameStartTime();
    // Gets delta time as sf::Time
    static sf::Time GetDeltaTimeAsSfTime();
    // Gets deltat time as float value
    static float GetDeltaTimeFloat();
};