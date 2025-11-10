#include "../stdafx.h"
#include "Scroll.h"

gui::Scroll::Scroll(float x, float y, float width, float height)
	: BaseGui(sf::Vector2f(x, y), sf::Vector2f(width, height))
{
	// Button Up
	this->buttonUp = new Button(x, y, width, width, "^", 20);
	this->buttonUp->onPressed(
		[this] {
			this->value--;
			this->updateIndicator();
			this->onValueChangeCallback();
		}
	);

	// Area
	this->shape.setFillColor(sf::Color::Blue);
	this->shape.setOutlineThickness(1.f);
	this->shape.setOutlineColor(sf::Color::Black);

	// Button Down
	this->buttonDown = new Button(x, height - width, width, width, "v", 20);
	this->buttonDown->onPressed(
		[this] {
			this->value++;
			this->updateIndicator();
			this->onValueChangeCallback();
		}
	);

	// Indicator
	this->indicatorShape.setFillColor(sf::Color::Black);

	this->indicatorPressed = false;

	this->onValueChangeCallback = [] {};

	this->indicatorHeight = width * 2;

	this->setSize(width, height);
	this->setPosition(x, y);
}

gui::Scroll::~Scroll()
{
	delete this->buttonUp;
	delete this->buttonDown;
}

void gui::Scroll::setPosition(const float x, const float y)
{
	BaseGui::setPosition(x,y);

	this->buttonUp->setPosition(x, y);
	this->shape.setPosition(sf::Vector2f(x, y + this->buttonUp->getSize().y));
	this->buttonDown->setPosition(x, this->shape.getPosition().y + this->shape.getSize().y);

	this->updateIndicator();
}

void gui::Scroll::setSize(const float width, const float height)
{
	BaseGui::setSize(width, height);

	this->buttonUp->setSize(width, width);
	this->shape.setSize(sf::Vector2f(width, height - 2 * width));
	this->buttonDown->setSize(width, width);

	this->indicatorShape.setSize(sf::Vector2f(width, this->indicatorHeight));
}

void gui::Scroll::setMinValue(int value)
{
	this->minValue = value;
}

void gui::Scroll::setMaxValue(int value)
{
	this->maxValue = value;
}

int gui::Scroll::getMinValue()
{
	return this->minValue;
}

int gui::Scroll::getMaxValue()
{
	return this->maxValue;
}

void gui::Scroll::setIndicatorHeight(float height)
{
	this->indicatorHeight = height;
}

void gui::Scroll::onValueChange(std::function<void()> callback)
{
	this->onValueChangeCallback = callback;
}

void gui::Scroll::updateIndicator()
{
	if (this->value < this->minValue)
	{
		this->value = this->minValue;
		return;
	}
	else if (this->value > this->maxValue)
	{
		this->value = this->maxValue;
		return;
	}

	float value_perc = static_cast<float>(this->value - this->minValue) / static_cast<float>(this->maxValue - this->minValue);

	this->indicatorShape.setPosition(
		sf::Vector2f(
			this->shape.getPosition().x + (this->shape.getSize().x / 2.f) - (this->indicatorShape.getGlobalBounds().size.x / 2.f),
			this->shape.getPosition().y + (this->shape.getSize().y - this->indicatorShape.getGlobalBounds().size.y) * value_perc
		)
	);
}

void gui::Scroll::scrollWheel(int delta)
{
	this->value -= delta*this->step;
	this->updateIndicator();
	this->onValueChangeCallback();
}

int gui::Scroll::getValue()
{
	return this->value;
}

void gui::Scroll::updateEvents(sf::Event& sfEvent, const sf::Vector2f& mousePos)
{
	this->buttonUp->updateEvents(sfEvent, mousePos);
	this->buttonDown->updateEvents(sfEvent, mousePos);

	if (this->indicatorShape.getGlobalBounds().contains(mousePos)) {
		if (auto mouseEvent = sfEvent.getIf<sf::Event::MouseButtonPressed>())
		{
			if (mouseEvent->button == sf::Mouse::Button::Left)
			{
				this->indicatorPressed = true;
			}
		}
	}

	if (this->indicatorPressed)
	{
		if (auto mouseEvent = sfEvent.getIf<sf::Event::MouseButtonReleased>())
		{
			if (mouseEvent->button == sf::Mouse::Button::Left)
			{
				this->indicatorPressed = false;
			}
		}
		else if (auto mouseEvent = sfEvent.getIf<sf::Event::MouseMoved>())
		{
			float interval_size = (this->shape.getSize().y / (static_cast<float>(this->maxValue - this->minValue) / this->step));
			float local_mouse_y = mousePos.y - this->shape.getPosition().y + (interval_size / 2.f);

			if (local_mouse_y <= 0.f)
				local_mouse_y = 0.f;
			else if (local_mouse_y >= this->shape.getSize().y)
				local_mouse_y = this->shape.getSize().y;

			int new_val = static_cast<int>(local_mouse_y / interval_size) * this->step + this->minValue;

			// Only update if the value changes
			if (new_val != this->value)
			{
				this->value = new_val;
				this->updateIndicator();
				this->onValueChangeCallback();
			}
		}
	}

	if (auto mouseEvent = sfEvent.getIf<sf::Event::MouseWheelScrolled>())
	{
		if (this->contains(mousePos))
		{
			this->scrollWheel(static_cast<int>(mouseEvent->delta));
		}
	}
}

void gui::Scroll::update(const sf::Vector2f& mousePos)
{
	this->buttonUp->update(mousePos);
	this->buttonDown->update(mousePos);
}

void gui::Scroll::render(sf::RenderTarget& target)
{
	target.draw(this->shape);
	this->buttonUp->render(target);
	this->buttonDown->render(target);
	target.draw(this->indicatorShape);
}
