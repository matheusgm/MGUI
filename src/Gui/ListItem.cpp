#include "../stdafx.h"
#include "ListItem.h"

gui::ListItem::ListItem(sf::Color color)
	: BaseGui({0.f, 0.f}, {100.f, 50.f})
{
	// Background Shape
	shape.setFillColor(color);
	shape.setOutlineThickness(1.f);
	shape.setOutlineColor(sf::Color::Black);

	// Text
	// text.setFont(*font);
	// text.setFillColor(sf::Color::White);

	setSize(getWidth(), getHeight());
	setPosition(getLeft(), getTop());
}

void gui::ListItem::setPosition(float x, float y)
{
	BaseGui::setPosition(x, y);
	shape.setPosition(getPosition());
	// text.setPosition(getPosition());
}

void gui::ListItem::setSize(float x, float y)
{
	BaseGui::setSize(x, y);
	shape.setSize(getSize());
}

void gui::ListItem::render(sf::RenderTarget &target)
{
	target.draw(shape);
	// target.draw(text);
}

void gui::ListItem::updateEvents(sf::Event &sfEvent, const sf::Vector2f &mousePos)
{
}

void gui::ListItem::update(const sf::Vector2f &mousePos)
{
}
