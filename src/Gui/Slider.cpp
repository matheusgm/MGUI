#include "../stdafx.hpp"
#include "Slider.hpp"

gui::Slider::Slider(sf::Vector2f position, sf::Vector2f size,
					int min_value, int max_value, int default_value, int step,
					sf::Color background_color, sf::Color foreground_color, sf::Color indicator_color) : GuiElement(position, size),
																										 step(step),
																										 minValue(min_value),
																										 maxValue(max_value),
																										 value(default_value)
{
	// Background Shape
	backgroundShape.setSize(size);
	backgroundShape.setFillColor(background_color);
	backgroundShape.setOutlineThickness(1.f);
	backgroundShape.setOutlineColor(sf::Color::Black);

	// Foreground Shape
	foregroundShape.setSize({0.f, size.y});
	foregroundShape.setFillColor(foreground_color);

	// Indicator
	indicatorShape.setRadius(size.y * 0.7f);
	indicatorShape.setFillColor(indicator_color);
	indicatorShape.setOutlineThickness(1.f);
	indicatorShape.setOutlineColor(sf::Color::Black);

	updateIndicatorPosition();
}

void gui::Slider::updateIndicatorPosition()
{
	float range = std::max(1, maxValue - minValue);
	float perc = float(value - minValue) / float(range);
	perc = std::clamp(perc, 0.f, 1.f);

	sf::Vector2f shapeSize = backgroundShape.getSize();

	foregroundShape.setSize({shapeSize.x * perc, shapeSize.y});

	// 1. Posição X: Posição do Slider (0 local) + Perc * Width
	float indicatorX = shapeSize.x * perc;

	// 2. Posição Y: Centro do Slider local
	float indicatorY = shapeSize.y / 2.f;

	float radius = indicatorShape.getRadius();
	indicatorShape.setOrigin({radius, radius});

	indicatorShape.setPosition({indicatorX, indicatorY});
}

void gui::Slider::handleDrag(const sf::Vector2f &mousePos)
{
	float trackLeft = getPosition().x;
	float trackWidth = backgroundShape.getSize().x;

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

void gui::Slider::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
	states.transform *= this->getTransform();

	target.draw(backgroundShape, states);
	target.draw(foregroundShape, states);
	target.draw(indicatorShape, states);
}

void gui::Slider::updateEvents(sf::Event &sfEvent, const sf::Vector2f &mousePos)
{
	sf::Transform indicatorTotalTransform = getTransform() * indicatorShape.getTransform();

	sf::FloatRect indicatorGlobalBounds = indicatorTotalTransform.transformRect(indicatorShape.getLocalBounds());

	if (indicatorGlobalBounds.contains(mousePos))
	{
		if (auto mouseEvent = sfEvent.getIf<sf::Event::MouseButtonPressed>())
		{
			if (mouseEvent->button == sf::Mouse::Button::Left)
			{
				indicatorPressed = true;
				sf::Vector2f indicatorCenterGlobal = getTransform().transformPoint(
					indicatorShape.getPosition());
				dragOffsetX = mousePos.x - indicatorCenterGlobal.x;
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

sf::FloatRect gui::Slider::getGlobalBounds() const
{
	sf::FloatRect localBounds = backgroundShape.getLocalBounds();
	return getTransform().transformRect(localBounds);
}

void gui::Slider::setSize(const sf::Vector2f &newSize)
{
	backgroundShape.setSize(newSize);

	updateIndicatorPosition();
}
