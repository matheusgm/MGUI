#include "../stdafx.hpp"
#include "Button.hpp"

std::unique_ptr<sf::Font> gui::Button::defaultFont = nullptr;

gui::Button::Button(sf::Vector2f position, sf::Vector2f size,
					sf::Font *font, const std::string &textStr, unsigned charSize,
					sf::Color textNormal, sf::Color textHover, sf::Color textPressed,
					sf::Color normal, sf::Color hover, sf::Color pressed,
					sf::Color outlineNormal, sf::Color outlineHover, sf::Color outlinePressed,
					short unsigned id) : GuiElement(position),
										 text(loadFont(font), textStr),
										 buttonState(ButtonState::NORMAL),
										 m_isDisabled(false),
										 textNormalColor(textNormal),
										 textHoverColor(textHover),
										 textPressedColor(textPressed),
										 textDisabledColor(sf::Color(textNormal.r, textNormal.g, textNormal.b, 150)),
										 normalColor(normal),
										 hoverColor(hover),
										 pressedColor(pressed),
										 disabledColor(sf::Color(normal.r, normal.g, normal.b, 150)),
										 outlineNormalColor(outlineNormal),
										 outlineHoverColor(outlineHover),
										 outlinePressedColor(outlinePressed),
										 outlineDisabledColor(sf::Color(outlinePressed.r, outlinePressed.g, outlinePressed.b, 150))
{
	// Shape
	shape.setSize(size);
	shape.setFillColor(normalColor);
	shape.setOutlineThickness(1.f);
	shape.setOutlineColor(outlineNormal);

	// Text
	text.setFillColor(textNormal);
	text.setCharacterSize(charSize);
	setText(textStr);
}

void gui::Button::update(sf::Time deltaTime)
{
}

void gui::Button::handleMouseInput(sf::Event event, const sf::Vector2f &mousePos)
{
	if (m_isDisabled || m_isPressed)
		return;

	bool hovered = isHovered(mousePos);

	if (hovered)
	{
		if (buttonState != ButtonState::HOVER)
		{
			buttonState = ButtonState::HOVER;
			updateVisualState();
		}
	}
	else if (buttonState == ButtonState::HOVER)
	{
		buttonState = m_isFocused ? ButtonState::FOCUSED : ButtonState::NORMAL;
		updateVisualState();
	}
}

sf::FloatRect gui::Button::getLocalBounds() const
{
	return shape.getLocalBounds();
}

void gui::Button::executeClickAction()
{
	if (m_isDisabled)
		return;

	onPressedCallback();
}

void gui::Button::setPressedState(bool pressed, const sf::Vector2f &mousePos)
{
	m_isPressed = pressed;
	if (m_isDisabled)
		return;

	if (pressed)
		buttonState = ButtonState::PRESSED;
	else
	{
		if (isHovered(mousePos))
			buttonState = m_isFocused ? ButtonState::FOCUSED : ButtonState::HOVER;
		else
			buttonState = m_isFocused ? ButtonState::FOCUSED : ButtonState::NORMAL;
	}
	updateVisualState();
}

void gui::Button::setText(const std::string &textStr)
{
	text.setString(textStr);
	auto bounds = text.getLocalBounds();
	text.setOrigin({bounds.position.x + bounds.size.x / 2.f, bounds.position.y + bounds.size.y / 2.f});
	text.setPosition({shape.getSize().x / 2.f, shape.getSize().y / 2.f});
}

void gui::Button::setDisabled(bool disable)
{
	m_isDisabled = disable;

	buttonState = disable ? ButtonState::DISABLED : ButtonState::NORMAL;
	updateVisualState();
}

void gui::Button::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
	states.transform *= getTransform();

	target.draw(shape, states);
	target.draw(text, states);
}

// bool gui::Button::isHovered(const sf::Vector2f &mousePos) const
// {
// 	sf::FloatRect boundsSpace = getTransform().transformRect(shape.getLocalBounds());

// 	return boundsSpace.contains(mousePos);
// }

void gui::Button::centerText()
{
}

void gui::Button::updateVisualState()
{
	switch (buttonState)
	{
	case ButtonState::NORMAL:
		shape.setFillColor(normalColor);
		text.setFillColor(textNormalColor);
		shape.setOutlineColor(outlineNormalColor);
		break;
	case ButtonState::HOVER:
		shape.setFillColor(hoverColor);
		text.setFillColor(textHoverColor);
		shape.setOutlineColor(outlineHoverColor);
		break;
	case ButtonState::PRESSED:
		shape.setFillColor(pressedColor);
		text.setFillColor(textPressedColor);
		shape.setOutlineColor(outlinePressedColor);
		break;
	case ButtonState::FOCUSED:
		shape.setFillColor(normalColor);
		text.setFillColor(textNormalColor);
		shape.setOutlineColor(sf::Color::Blue);
		break;
	case ButtonState::DISABLED:
		shape.setFillColor(disabledColor);
		text.setFillColor(textDisabledColor);
		shape.setOutlineColor(outlineDisabledColor);
		break;
	default:
		shape.setFillColor(sf::Color::Red);
		text.setFillColor(sf::Color::Blue);
		shape.setOutlineColor(sf::Color::Green);
		break;
	}
}

sf::Font &gui::Button::loadFont(sf::Font *font)
{
	if (font)
		return *font;

	if (!defaultFont)
	{
		defaultFont = std::make_unique<sf::Font>();
		if (!defaultFont->openFromFile("src/Fonts/MochiyPopPOne-Regular.ttf"))
			throw std::runtime_error("Failed to load default font!");
	}

	return *defaultFont;
}
