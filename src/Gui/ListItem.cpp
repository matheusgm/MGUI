#include "../stdafx.h"
#include "ListItem.h"

gui::ListItem::ListItem(sf::Color color)
	:BaseGui(sf::Vector2f(0.f, 0.f), sf::Vector2f(100.f, 50.f))
{
	// Background Shape
	this->shape.setFillColor(color);
	this->shape.setOutlineThickness(1.f);
	this->shape.setOutlineColor(sf::Color::Black);

	// Text
	//this->text.setFont(*this->font);
	// this->text.setFillColor(sf::Color::White);

	this->setSize(getWidth(), getHeight());
	this->setPosition(getLeft(), getTop());
}

void gui::ListItem::setPosition(float x, float y)
{
	BaseGui::setPosition(x, y);
	this->shape.setPosition(this->getPosition());
	// this->text.setPosition(this->getPosition());
}

void gui::ListItem::setSize(float x, float y)
{
	BaseGui::setSize(x, y);
	this->shape.setSize(this->getSize());
}

void gui::ListItem::render(sf::RenderTarget& target)
{
	target.draw(this->shape);
	// target.draw(this->text);
}

void gui::ListItem::updateEvents(sf::Event& sfEvent, const sf::Vector2f& mousePos)
{
}

void gui::ListItem::update(const sf::Vector2f& mousePos)
{
}
