#include "../stdafx.hpp"
#include "Button.hpp"

std::unique_ptr<sf::Font> gui::Button::defaultFont = nullptr;

gui::Button::Button(sf::Vector2f position, sf::Vector2f size,
					sf::Font *font, const std::string &textStr, unsigned charSize,
					sf::Color textNormal, sf::Color textHover, sf::Color textPressed,
					sf::Color normal, sf::Color hover, sf::Color pressed,
					sf::Color outlineNormal, sf::Color outlineHover, sf::Color outlinePressed,
					short unsigned id) : GuiElement(position, size),
										 text(loadFont(font), textStr),
										 buttonState(ButtonState::NORMAL),
										 id(id),
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

void gui::Button::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
	states.transform *= this->getTransform();

	target.draw(shape, states);
	target.draw(text, states);
}

// Modifier
void gui::Button::setText(const std::string &textStr)
{
	text.setString(textStr);
	auto bounds = text.getLocalBounds();
	text.setOrigin({bounds.position.x + bounds.size.x / 2.f, bounds.position.y + bounds.size.y / 2.f});
	text.setPosition({shape.getSize().x / 2.f, shape.getSize().y / 2.f});
}

void gui::Button::setDisabled(bool disable)
{
	buttonState = disable ? ButtonState::DISABLED : ButtonState::NORMAL;
}

sf::FloatRect gui::Button::getGlobalBounds() const
{
	// With Shape Outline Thickness
	sf::FloatRect localBounds = shape.getLocalBounds();
	return getTransform().transformRect(localBounds);

	// Without Shape Outline Thickness
	// sf::Vector2f globalPos = this->getPosition();

	// sf::Vector2f size = shape.getSize();

	// return sf::FloatRect({globalPos.x, globalPos.y}, {size.x, size.y});
}

bool gui::Button::isHovered(const sf::Vector2f &mousePos) const
{
	sf::FloatRect boundsSpace = getTransform().transformRect(shape.getLocalBounds());

	return boundsSpace.contains(mousePos);
}

// Functions
void gui::Button::updateEvents(sf::Event &sfEvent, const sf::Vector2f &mousePos)
{
	if (buttonState == ButtonState::DISABLED)
		return;

	bool hovered = isHovered(mousePos);

	// Check MouseButtonPressed
	if (auto mousePressed = sfEvent.getIf<sf::Event::MouseButtonPressed>())
	{
		if (mousePressed->button == sf::Mouse::Button::Left && hovered)
		{
			buttonState = ButtonState::PRESSED;
			buttonPressed = true;
		}
	}

	// Check MouseButtonReleased
	if (auto mouseReleased = sfEvent.getIf<sf::Event::MouseButtonReleased>())
	{
		if (mouseReleased->button == sf::Mouse::Button::Left && buttonPressed)
		{
			if (hovered)
				onPressedCallback();

			buttonReleased = true;
			buttonPressed = false;
			buttonState = hovered ? ButtonState::HOVER : ButtonState::NORMAL;
		}
	}

	// Update hover state if not pressed
	if (!buttonPressed)
		buttonState = hovered ? ButtonState::HOVER : ButtonState::NORMAL;
}

void gui::Button::update(const sf::Vector2f &mousePos)
{
	if (buttonReleased)
	{
		buttonReleased = false;
		buttonState = ButtonState::NORMAL;
		bool hovered = isHovered(mousePos);
		if (hovered)
			buttonState = ButtonState::HOVER;
	}

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