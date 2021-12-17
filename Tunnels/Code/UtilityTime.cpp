#include "UtilityTime.h"

void UtilityTime::FrameStartTime()
{
    _setTime = _deltaClock.restart();
}

sf::Time UtilityTime::GetDeltaTimeAsSfTime()
{
    return _setTime;
}

float UtilityTime::GetDeltaTimeFloat()
{
    return _setTime.asSeconds();
}

// Initialize static member
sf::Time UtilityTime::_setTime;
sf::Clock UtilityTime::_deltaClock;