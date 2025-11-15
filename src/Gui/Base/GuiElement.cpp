#include "../../stdafx.hpp"
#include "GuiElement.hpp"

gui::GuiElement::GuiElement(sf::Vector2f position)
{
	setPosition(position);
}

sf::FloatRect gui::GuiElement::RectUnion(const sf::FloatRect &a, const sf::FloatRect &b) const
{
	float minX = std::min(a.position.x, b.position.x);
	float minY = std::min(a.position.y, b.position.y);

	float maxX = std::max(a.position.x + a.size.x, b.position.x + b.size.x);
	float maxY = std::max(a.position.y + a.size.y, b.position.x + b.size.y);

	return sf::FloatRect({minX, minY}, {maxX - minX, maxY - minY});
}