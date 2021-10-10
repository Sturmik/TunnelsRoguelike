#include "GameManager.h"

GameManager::GameManager() 
{
	// Initialize variables
}

void GameManager::Start()
{
	// Open window
	_windowManager.OpenWindow(sf::VideoMode(800, 600), "Tunnels");
}

void GameManager::UpdateState()
{

}