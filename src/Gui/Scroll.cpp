#include "../stdafx.hpp"
#include "Scroll.hpp"

gui::Scroll::Scroll(sf::Vector2f position, sf::Vector2f size)
	: GuiElement(position)
{
	// Button Up
	buttonUp = std::make_unique<Button>(sf::Vector2f(0.f, 0.f), sf::Vector2f(size.x, size.x), "^", size.x * 1.f);
	buttonUp->onPressed(
		[this]
		{
			value--;
			clampValue();
			updateIndicatorPosition();
			if (onValueChangeCallback)
				onValueChangeCallback();
		});

	float trackAreaY = size.y - 2 * size.x;

	// Area
	shape.setSize({size.x, trackAreaY});
	shape.setPosition(shape.getPosition() + sf::Vector2f({0.f, size.x}));
	shape.setFillColor(sf::Color::Blue);
	shape.setOutlineThickness(1.f);
	shape.setOutlineColor(sf::Color::Black);

	// Button Down
	buttonDown = std::make_unique<Button>(sf::Vector2f(0.f, size.x + trackAreaY), sf::Vector2f(size.x, size.x), "^", size.x * 1.f);
	buttonDown->setOrigin({size.x / 2, size.x / 2});
	buttonDown->setRotation(sf::degrees(180.f));
	buttonDown->setPosition(buttonDown->getPosition() + sf::Vector2f({size.x / 2, size.x / 2}));
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

void gui::Scroll::handleMouseInput(sf::Event event, const sf::Vector2f &mousePos)
{
	sf::Vector2f scrollLocalMousePos = mapGlobalToLocal(mousePos);

	// cout << "X: " << mousePos.x << " Y: " << mousePos.y << endl;

	buttonUp->handleMouseInput(event, scrollLocalMousePos);
	buttonDown->handleMouseInput(event, scrollLocalMousePos);

	if (m_thumbPressed)
		handleDrag(scrollLocalMousePos);

	// 3. Lógica do HOVER na Calha (Opcional)
	// Se quiser que a calha mude de cor ao passar o mouse.
}

void gui::Scroll::update(sf::Time deltaTime)
{
	buttonUp->update(deltaTime);
	buttonDown->update(deltaTime);
}

sf::FloatRect gui::Scroll::getLocalBounds() const
{
	sf::FloatRect buttonUpArea = buttonUp->getTransform().transformRect(buttonUp->getLocalBounds());
	sf::FloatRect shapeArea = shape.getTransform().transformRect(shape.getLocalBounds());
	sf::FloatRect buttonDownArea = buttonDown->getTransform().transformRect(buttonDown->getLocalBounds());

	sf::FloatRect combinedBounds = RectUnion(shapeArea, buttonUpArea);

	return RectUnion(combinedBounds, buttonDownArea);
}

void gui::Scroll::setPressedState(bool pressed, const sf::Vector2f &mousePos)
{
	sf::Vector2f scrollLocalMousePos = mapGlobalToLocal(mousePos);

	GuiElement *childUnderMouse = findChildAt(scrollLocalMousePos);

	if (pressed)
	{
		if (childUnderMouse)
		{
			if (IPressable *pressableChild = dynamic_cast<IPressable *>(childUnderMouse))
			{
				pressableChild->setPressedState(true, scrollLocalMousePos);
				m_pressedChild = childUnderMouse;
				return;
			}
		}

		sf::FloatRect indicatorLocalBounds = indicatorShape.getTransform().transformRect(indicatorShape.getLocalBounds());

		if (indicatorLocalBounds.contains(scrollLocalMousePos))
		{
			m_isPressed = true;
			m_thumbPressed = true;
			dragOffsetY = scrollLocalMousePos.y - indicatorShape.getPosition().y;
			return;
		}

		// --- 3. Calha (Track) ---
		// Se não foi Thumb nem Button, o Scroll (Track) pode ser considerado pressionado.
		m_isPressed = true;
		// ... (Lógica para rolar uma página, se aplicável)
	}
	else
	{
		if (m_pressedChild)
		{
			if (childUnderMouse == m_pressedChild)
			{
				if (IClickable *clickableChild = dynamic_cast<IClickable *>(m_pressedChild))
					clickableChild->executeClickAction();
			}

			if (IPressable *pressableChild = dynamic_cast<IPressable *>(m_pressedChild))
				pressableChild->setPressedState(false, scrollLocalMousePos);
		}

		// Reseta todos os estados
		m_isPressed = false;
		m_thumbPressed = false;
		dragOffsetY = 0.0f;
		m_pressedChild = nullptr;

		buttonUp->setPressedState(false, scrollLocalMousePos);
		buttonDown->setPressedState(false, scrollLocalMousePos);
	}
}

void gui::Scroll::scrollWheel(int delta)
{
	value -= delta * step;
	clampValue();
	updateIndicatorPosition();
	if (onValueChangeCallback)
		onValueChangeCallback();
}

void gui::Scroll::setIndicatorHeightRatio(float ratio)
{
	ratio = std::clamp(ratio, 0.0f, 1.0f);

	float trackHeight = getTrackHeight();

	float minIndicatorHeight = getButtonUpHeight();
	float newHeight = trackHeight * ratio;

	indicatorHeight = std::clamp(newHeight, minIndicatorHeight, trackHeight);

	indicatorShape.setSize({shape.getSize().x, indicatorHeight});

	updateIndicatorPosition();
}

void gui::Scroll::setMinValue(int value)
{
	minValue = value;
	clampValue();
	updateIndicatorPosition();
}

void gui::Scroll::setMaxValue(int value)
{
	maxValue = value;
	clampValue();
	updateIndicatorPosition();
}

void gui::Scroll::setValue(int value)
{
	this->value = value;
	clampValue();
	updateIndicatorPosition();
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

void gui::Scroll::clampValue()
{
	value = std::clamp(value, minValue, maxValue);
}

void gui::Scroll::updateIndicatorPosition()
{
	float range = std::max(1, maxValue - minValue);
	float perc = float(value - minValue) / float(range);
	perc = std::clamp(perc, 0.f, 1.f);

	float trackHeight = getTrackHeight();
	float trackTop = getButtonUpHeight();

	float maxIndicatorMovement = trackHeight - indicatorHeight;

	float indicatorLocalX = 0.f;
	float indicatorLocalY = trackTop + (maxIndicatorMovement * perc);

	indicatorShape.setPosition({indicatorLocalX, indicatorLocalY});
}

void gui::Scroll::handleDrag(const sf::Vector2f &mousePos)
{
	float btnUpSizeY = getButtonUpHeight();
	sf::Vector2f indicatorSize = indicatorShape.getSize();

	// Altura da área de rolagem livre
	float trackHeight = getTrackHeight();

	// 1. Calcular a Posição Y do TOPO do indicador
	// Topo do Indicador Desejado = Mouse Y - Offset (distância do clique ao topo)
	float indicatorTopDesired = mousePos.y - dragOffsetY;

	float topLimit = btnUpSizeY;

	float bottomLimit = btnUpSizeY + trackHeight - indicatorSize.y;

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

gui::GuiElement *gui::Scroll::findChildAt(const sf::Vector2f &mousePos)
{
	if (buttonDown->contains(mousePos))
		return buttonDown.get();

	if (buttonUp->contains(mousePos))
		return buttonUp.get();

	return nullptr;
}
