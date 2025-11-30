#include "../stdafx.hpp"
#include "Select.hpp"

gui::Select::Select(sf::Vector2f position, sf::Vector2f size)
	: GuiElement(position)
{
	// Shape
	shape.setSize(size);
	shape.setFillColor(sf::Color::Red);
	shape.setOutlineThickness(1.f);
	shape.setOutlineColor(sf::Color::Black);
}

void gui::Select::handleMouseInput(sf::Event event, const sf::Vector2f &mousePos)
{
}

void gui::Select::update(sf::Time deltaTime)
{
}

sf::FloatRect gui::Select::getLocalBounds() const
{
	return shape.getLocalBounds();
}

void gui::Select::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
	states.transform *= this->getTransform();

	target.draw(shape, states);
}
