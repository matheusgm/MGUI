#include "../../stdafx.hpp"
#include "GuiElement.hpp"

gui::GuiElement::GuiElement(sf::Vector2f position, sf::Vector2f size)
{
	setPosition(position);
}

bool gui::GuiElement::contains(const sf::Vector2f &points) const
{
	return this->getGlobalBounds().contains(points);
}
