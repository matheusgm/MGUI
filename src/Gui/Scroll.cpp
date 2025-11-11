#include "../stdafx.h"
#include "Scroll.h"

gui::Scroll::Scroll(float x, float y, float width, float height)
	: GuiElement({x, y}, {width, height}),
	  indicatorHeight(width * 2)
{
	// Button Up
	buttonUp = std::make_unique<Button>(x, y, width, width, "^", 20);
	buttonUp->onPressed(
		[this]
		{
			value--;
			clampValue();
			updateIndicatorPosition();
			if (onValueChangeCallback)
				onValueChangeCallback();
		});

	// Area
	shape.setFillColor(sf::Color::Blue);
	shape.setOutlineThickness(1.f);
	shape.setOutlineColor(sf::Color::Black);

	// Button Down
	buttonDown = std::make_unique<Button>(x, height - width, width, width, "v", 20);
	buttonDown->onPressed(
		[this]
		{
			value++;
			clampValue();
			updateIndicatorPosition();
			if (onValueChangeCallback)
				onValueChangeCallback();
		});

	// Indicator
	indicatorShape.setFillColor(sf::Color::Black);

	setSize(width, height);
	setPosition(x, y);
}

void gui::Scroll::setPosition(float x, float y)
{
	GuiElement::setPosition(x, y);

	buttonUp->setPosition(x, y);
	shape.setPosition({x, y + buttonUp->getSize().y});
	buttonDown->setPosition(x, shape.getPosition().y + shape.getSize().y);

	updateIndicatorPosition();
}

void gui::Scroll::setSize(float width, float height)
{
	GuiElement::setSize(width, height);

	buttonUp->setSize(width, width);
	shape.setSize({width, height - 2 * width});
	buttonDown->setSize(width, width);

	indicatorShape.setSize({width, indicatorHeight});

	updateIndicatorPosition();
}

void gui::Scroll::clampValue()
{
	value = std::clamp(value, minValue, maxValue);
}

void gui::Scroll::updateIndicatorPosition()
{
	float range = std::max(1, maxValue - minValue);
	float perc = float(value - minValue) / float(range);
	perc = std::clamp(perc, 0.f, 1.f);

	indicatorShape.setPosition(
		{shape.getPosition().x,
		 shape.getPosition().y + (shape.getSize().y - indicatorShape.getGlobalBounds().size.y) * perc});
}

void gui::Scroll::scrollWheel(int delta)
{
	value -= delta * step;
	clampValue();
	updateIndicatorPosition();
	if (onValueChangeCallback)
		onValueChangeCallback();
}

void gui::Scroll::handleDrag(const sf::Vector2f &mousePos)
{
	float trackTop = shape.getPosition().y;
	float trackHeight = shape.getSize().y - indicatorShape.getSize().y;

	float newY = mousePos.y - dragOffsetY;
	newY = std::clamp(newY, trackTop, trackTop + trackHeight);

	float perc = (newY - trackTop) / trackHeight;
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

void gui::Scroll::updateEvents(sf::Event &sfEvent, const sf::Vector2f &mousePos)
{
	buttonUp->updateEvents(sfEvent, mousePos);
	buttonDown->updateEvents(sfEvent, mousePos);

	if (indicatorShape.getGlobalBounds().contains(mousePos))
	{
		if (auto mouseEvent = sfEvent.getIf<sf::Event::MouseButtonPressed>())
		{
			if (mouseEvent->button == sf::Mouse::Button::Left)
			{
				indicatorPressed = true;
				dragOffsetY = mousePos.y - indicatorShape.getPosition().y;
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

	if (auto mouseEvent = sfEvent.getIf<sf::Event::MouseWheelScrolled>())
	{
		if (contains(mousePos))
			scrollWheel(static_cast<int>(mouseEvent->delta));
	}
}

void gui::Scroll::update(const sf::Vector2f &mousePos)
{
	buttonUp->update(mousePos);
	buttonDown->update(mousePos);
}

void gui::Scroll::render(sf::RenderTarget &target)
{
	target.draw(shape);
	buttonUp->render(target);
	buttonDown->render(target);
	target.draw(indicatorShape);
}
