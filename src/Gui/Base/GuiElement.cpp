#include "../../stdafx.h"
#include "GuiElement.h"

gui::GuiElement::GuiElement(sf::Vector2f position, sf::Vector2f size) : rect({position, size})
{
}

void gui::GuiElement::setPosition(const float x, const float y)
{
	rect = sf::FloatRect({x, y}, rect.size);
}

void gui::GuiElement::setSize(const float width, const float height)
{
	rect = sf::FloatRect(rect.position, {width, height});
}
