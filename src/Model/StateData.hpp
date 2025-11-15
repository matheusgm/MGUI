#pragma once

// Forward declaration of the State class
class State;

struct StateData
{
	float gridSize = 0.f;
	sf::RenderWindow *window = nullptr;
	GraphicsSettings *gfxSettings = nullptr;
	std::map<std::string, int> *supportedKeys = nullptr;
	std::stack<std::unique_ptr<State>> *states = nullptr;
};