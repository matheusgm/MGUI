#include "../stdafx.h"
#include "Select.h"

gui::Select::Select(float x, float y, float width, float height)
	: BaseGui(sf::Vector2f(x, y), sf::Vector2f(width, height))
{
	// Shape
	this->shape.setFillColor(sf::Color::Red);
	this->shape.setOutlineThickness(1.f);
	this->shape.setOutlineColor(sf::Color::Black);

	this->setSize(width, height);
	this->setPosition(x, y);
}

void gui::Select::setPosition(const float x, const float y)
{
	this->shape.setPosition(sf::Vector2f(x, y));
}

void gui::Select::setSize(const float width, const float height)
{
	this->shape.setSize(sf::Vector2f(width, height));
}

void gui::Select::updateEvents(sf::Event& sfEvent, const sf::Vector2f& mousePos)
{
}

void gui::Select::update(const sf::Vector2f& mousePos)
{
}

void gui::Select::render(sf::RenderTarget& target)
{
	target.draw(this->shape);
}
