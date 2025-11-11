#include "../stdafx.h"
#include "Select.h"

gui::Select::Select(float x, float y, float width, float height)
	: GuiElement(sf::Vector2f(x, y), sf::Vector2f(width, height))
{
	// Shape
	shape.setFillColor(sf::Color::Red);
	shape.setOutlineThickness(1.f);
	shape.setOutlineColor(sf::Color::Black);

	setSize(width, height);
	setPosition(x, y);
}

void gui::Select::setPosition(const float x, const float y)
{
	shape.setPosition(sf::Vector2f(x, y));
}

void gui::Select::setSize(const float width, const float height)
{
	shape.setSize(sf::Vector2f(width, height));
}

void gui::Select::updateEvents(sf::Event& sfEvent, const sf::Vector2f& mousePos)
{
}

void gui::Select::update(const sf::Vector2f& mousePos)
{
}

void gui::Select::render(sf::RenderTarget& target)
{
	target.draw(shape);
}
