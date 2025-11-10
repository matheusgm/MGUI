#include "../../stdafx.h"
#include "BaseGui.h"

gui::BaseGui::BaseGui(sf::Vector2f position, sf::Vector2f size)
{
	this->rect = sf::FloatRect(position, size);
}

const sf::Vector2f gui::BaseGui::getPosition() const
{
	return this->rect.position;
}

const sf::Vector2f gui::BaseGui::getSize() const
{
	return this->rect.size;
}

const float gui::BaseGui::getTop() const
{
	return this->rect.position.y;
}

const float gui::BaseGui::getBottom() const
{
	return this->rect.position.y + this->rect.size.y;
}

const float gui::BaseGui::getLeft() const
{
	return this->rect.position.x;
}

const float gui::BaseGui::getRight() const
{
	return this->rect.position.x + this->rect.size.x;
}

const float gui::BaseGui::getWidth() const
{
	return this->rect.size.x;
}

const float gui::BaseGui::getHeight() const
{
	return this->rect.size.y;
}

void gui::BaseGui::setPosition(const float x, const float y)
{
	this->rect = sf::FloatRect(sf::Vector2f(x,y), this->rect.size);
}

void gui::BaseGui::setSize(const float width, const float height)
{
	this->rect = sf::FloatRect(this->rect.position, sf::Vector2f(width, height));
}

bool gui::BaseGui::contains(const sf::Vector2f& points)
{
	return this->rect.contains(points);
}
