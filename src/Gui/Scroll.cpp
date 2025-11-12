#include "../stdafx.hpp"
#include "Scroll.hpp"

gui::Scroll::Scroll(sf::Vector2f position, sf::Vector2f size)
	: GuiElement(position, size),
	  indicatorHeight(size.x * 2)
{
	// Button Up
	buttonUp = std::make_unique<Button>(position, sf::Vector2f(size.x, size.x), "^", 20);
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
	shape.setSize(size);
	shape.setFillColor(sf::Color::Blue);
	shape.setOutlineThickness(1.f);
	shape.setOutlineColor(sf::Color::Black);
	shape.setPosition(position);

	// Button Down
	buttonDown = std::make_unique<Button>(sf::Vector2f(position.x, size.y - size.x), sf::Vector2f(size.x, size.x), "v", 20);
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

void gui::Scroll::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
	states.transform *= this->getTransform();

	target.draw(shape, states);
	if (buttonUp)
		target.draw(*buttonUp, states);
	if (buttonDown)
		target.draw(*buttonDown, states);
	target.draw(indicatorShape, states);
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

sf::FloatRect gui::Scroll::getGlobalBounds() const
{
	return shape.getGlobalBounds();
}
