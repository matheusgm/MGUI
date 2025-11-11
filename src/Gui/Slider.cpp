#include "../stdafx.h"
#include "Slider.h"

gui::Slider::Slider(float x, float y, float width, float height,
					int min_value, int max_value, int default_value, int step,
					sf::Color background_color, sf::Color foreground_color, sf::Color indicator_color) : BaseGui({x, y}, {width, height}),
																										 step(step),
																										 minValue(min_value),
																										 maxValue(max_value),
																										 value(default_value)
{
	// Background Shape
	backgroundShape.setFillColor(background_color);
	backgroundShape.setOutlineThickness(1.f);
	backgroundShape.setOutlineColor(sf::Color::Black);

	// Foreground Shape
	foregroundShape.setFillColor(foreground_color);

	// Indicator
	indicatorShape.setRadius(height * 0.7f);
	indicatorShape.setFillColor(indicator_color);
	indicatorShape.setOutlineThickness(1.f);
	indicatorShape.setOutlineColor(sf::Color::Black);

	setSize(width, height);
	setPosition(x, y);
}

void gui::Slider::setPosition(float x, float y)
{
	BaseGui::setPosition(x, y);

	backgroundShape.setPosition(getPosition());
	foregroundShape.setPosition(getPosition());

	updateIndicatorPosition();
}

void gui::Slider::setSize(float width, float height)
{
	BaseGui::setSize(width, height);

	backgroundShape.setSize(getSize());

	updateIndicatorPosition();
}

void gui::Slider::updateIndicatorPosition()
{
	float range = std::max(1, maxValue - minValue);
	float perc = float(value - minValue) / float(range);
	perc = std::clamp(perc, 0.f, 1.f);

	foregroundShape.setSize({getSize().x * perc, getSize().y});

	indicatorShape.setPosition(
		{getPosition().x - (indicatorShape.getGlobalBounds().size.x / 2.f) + (getSize().x * perc),
		 getPosition().y + (getSize().y / 2.f) - (indicatorShape.getGlobalBounds().size.y / 2.f)});
}

void gui::Slider::handleDrag(const sf::Vector2f &mousePos)
{
	float trackLeft = getPosition().x;
	float trackWidth = getSize().x;

	float newX = mousePos.x - dragOffsetX;
	newX = std::clamp(newX, trackLeft, trackLeft + trackWidth);

	float perc = (newX - trackLeft) / trackWidth;
	int range = std::max(1, maxValue - minValue);
	int newVal = minValue + static_cast<int>(perc * range);

	if (newVal != value)
	{
		value = newVal;
		updateIndicatorPosition();
		if (onValueChangeCallback)
			onValueChangeCallback();
	}
}

void gui::Slider::updateEvents(sf::Event &sfEvent, const sf::Vector2f &mousePos)
{
	if (indicatorShape.getGlobalBounds().contains(mousePos))
	{
		if (auto mouseEvent = sfEvent.getIf<sf::Event::MouseButtonPressed>())
		{
			if (mouseEvent->button == sf::Mouse::Button::Left)
			{
				indicatorPressed = true;
				dragOffsetX = mousePos.x - indicatorShape.getPosition().x;
			}
		}
	}

	if (indicatorPressed)
	{
		if (auto mouseEvent = sfEvent.getIf<sf::Event::MouseButtonReleased>())
		{
			if (mouseEvent->button == sf::Mouse::Button::Left)
				indicatorPressed = false;
		}
		else if (auto mouseEvent = sfEvent.getIf<sf::Event::MouseMoved>())
			handleDrag(mousePos);
	}
}

void gui::Slider::update(const sf::Vector2f &mousePos)
{
}

void gui::Slider::render(sf::RenderTarget &target)
{
	target.draw(backgroundShape);
	target.draw(foregroundShape);
	target.draw(indicatorShape);
}
