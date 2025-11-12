#include "../stdafx.hpp"
#include "Scroll.hpp"

gui::Scroll::Scroll(sf::Vector2f position, sf::Vector2f size)
	: GuiElement(position, size),
	  indicatorHeight(size.x * 2)
{
	// Button Up
	buttonUp = std::make_unique<Button>(sf::Vector2f(0.f, 0.f), sf::Vector2f(size.x, size.x), "^", 20);
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

	// Button Down
	buttonDown = std::make_unique<Button>(sf::Vector2f(0.f, size.y - size.x), sf::Vector2f(size.x, size.x), "v", 20);
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
	indicatorShape.setSize({size.x, indicatorHeight});

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

	sf::Vector2f shapeSize = shape.getSize();
	sf::Vector2f btnUpSize = buttonUp->getGlobalBounds().size;

	// Altura da área livre para o indicador (Track)
	float trackHeight = shapeSize.y - (2 * btnUpSize.y);

	// Posição Y Mínima do indicador (após o botão Up)
	float trackTop = btnUpSize.y;

	// Altura máxima que o indicador pode se mover
	float maxIndicatorMovement = trackHeight - indicatorHeight;

	// Posição X (local)
	float indicatorX = 0.f;

	// Posição final Y (local)
	float indicatorY = trackTop + (maxIndicatorMovement * perc);

	indicatorShape.setPosition({indicatorX, indicatorY});
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
	sf::Vector2f scrollGlobalPos = getPosition();
	sf::Vector2f btnUpSize = buttonUp->getGlobalBounds().size;
	sf::Vector2f indicatorSize = indicatorShape.getSize();

	// Início da área de rolagem livre (Global)
	float trackTopGlobal = scrollGlobalPos.y + btnUpSize.y;
	// Altura da área de rolagem livre
	float trackHeight = shape.getSize().y - (2 * btnUpSize.y);

	// 1. Calcular a Posição Y do TOPO do indicador
	// Topo do Indicador Desejado = Mouse Y - Offset (distância do clique ao topo)
	float indicatorTopDesired = mousePos.y - dragOffsetY;

	float topLimit = trackTopGlobal;

	float bottomLimit = trackTopGlobal + trackHeight - indicatorSize.y;

	float clampedTopY = std::clamp(indicatorTopDesired, topLimit, bottomLimit);

	// 3. Calcular a Porcentagem (Perc) a partir do TOPO (clampedTopY)
	// Posição de Referência Zero: topLimit (representa 0% de rolagem)
	float distanceMoved = clampedTopY - topLimit;

	// Altura Máxima de Movimento
	float maxMovement = bottomLimit - topLimit;

	float perc = distanceMoved / maxMovement;
	perc = std::clamp(perc, 0.f, 1.f);

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
	sf::Vector2f localMousePos = this->getInverseTransform().transformPoint(mousePos);

	buttonUp->updateEvents(sfEvent, localMousePos);
	buttonDown->updateEvents(sfEvent, localMousePos);

	sf::Transform indicatorTotalTransform = getTransform() * indicatorShape.getTransform();

	sf::FloatRect indicatorGlobalBounds = indicatorTotalTransform.transformRect(indicatorShape.getLocalBounds());

	if (indicatorGlobalBounds.contains(mousePos))
	{
		if (auto mouseEvent = sfEvent.getIf<sf::Event::MouseButtonPressed>())
		{
			if (mouseEvent->button == sf::Mouse::Button::Left)
			{
				indicatorPressed = true;
				sf::Vector2f indicatorTopGlobal = getTransform().transformPoint(
					indicatorShape.getPosition());

				dragOffsetY = mousePos.y - indicatorTopGlobal.y;
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
	sf::FloatRect localBounds = shape.getLocalBounds();
	return getTransform().transformRect(localBounds);
}
