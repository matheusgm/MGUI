#include "stdafx.h"
#include "State.h"

State::State(StateData &state_data) : data(state_data),
									  font("src/Fonts/MochiyPopPOne-Regular.ttf")
{
}

bool State::getKeytime()
{
	if (this->keytime >= this->keytimeMax)
	{
		this->keytime = 0.f;
		return true;
	}
	return false;
}

sf::Vector2f State::getWindowCenter() const
{
	return sf::Vector2f(
		static_cast<float>(this->data.window->getSize().x / 2.f),
		static_cast<float>(this->data.window->getSize().y / 2.f));
}

void State::updateMousePositions()
{
	this->mousePosScreen = sf::Mouse::getPosition();
	this->mousePosWindow = sf::Mouse::getPosition(*this->data.window);
	this->mousePosView = this->data.window->mapPixelToCoords(sf::Mouse::getPosition(*this->data.window));
}

void State::updateKeytime(float dt)
{
	if (this->keytime < this->keytimeMax)
	{
		this->keytime += 100.f * dt;
	}
}
