#include "../../stdafx.h"
#include "GuiElement.h"

gui::GuiElement::GuiElement(sf::Vector2f position, sf::Vector2f size)
{
	rect = sf::FloatRect(position, size);
}

const sf::Vector2f gui::GuiElement::getPosition() const
{
	return rect.position;
}

const sf::Vector2f gui::GuiElement::getSize() const
{
	return rect.size;
}

const float gui::GuiElement::getTop() const
{
	return rect.position.y;
}

const float gui::GuiElement::getBottom() const
{
	return rect.position.y + rect.size.y;
}

const float gui::GuiElement::getLeft() const
{
	return rect.position.x;
}

const float gui::GuiElement::getRight() const
{
	return rect.position.x + rect.size.x;
}

const float gui::GuiElement::getWidth() const
{
	return rect.size.x;
}

const float gui::GuiElement::getHeight() const
{
	return rect.size.y;
}

void gui::GuiElement::setPosition(const float x, const float y)
{
	rect = sf::FloatRect(sf::Vector2f(x,y), rect.size);
}

void gui::GuiElement::setSize(const float width, const float height)
{
	rect = sf::FloatRect(rect.position, sf::Vector2f(width, height));
}

bool gui::GuiElement::contains(const sf::Vector2f& points)
{
	return rect.contains(points);
}
