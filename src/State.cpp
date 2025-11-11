#include "stdafx.h"
#include "State.h"

State::State(StateData &state_data) : data(state_data),
									  font("src/Fonts/MochiyPopPOne-Regular.ttf")
{
}

bool State::getKeytime()
{
	if (keytime >= keytimeMax)
	{
		keytime = 0.f;
		return true;
	}
	return false;
}

sf::Vector2f State::getWindowCenter() const
{
	return sf::Vector2f(
		static_cast<float>(data.window->getSize().x / 2.f),
		static_cast<float>(data.window->getSize().y / 2.f));
}

void State::updateMousePositions()
{
	mousePosScreen = sf::Mouse::getPosition();
	mousePosWindow = sf::Mouse::getPosition(*data.window);
	mousePosView = data.window->mapPixelToCoords(sf::Mouse::getPosition(*data.window));
}

void State::updateKeytime(float dt)
{
	if (keytime < keytimeMax)
	{
		keytime += 100.f * dt;
	}
}
