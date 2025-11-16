#pragma once

#include "Base/GuiElement.hpp"

namespace gui
{
	enum class ButtonState : uint8_t
	{
		NORMAL,
		HOVER,
		PRESSED,
		DISABLED
	};

	class Button : public GuiElement
	{
	public:
		Button(sf::Vector2f position, sf::Vector2f size,
			   sf::Font *font = nullptr, const std::string &textStr = "Text", unsigned character_size = 24,
			   sf::Color text_normal_color = sf::Color::Black, sf::Color text_hover_color = sf::Color::White, sf::Color text_pressed_color = sf::Color(200, 200, 200, 255),
			   sf::Color normal_color = sf::Color::White, sf::Color hover_color = sf::Color::Black, sf::Color pressed_color = sf::Color(60, 60, 60, 255),
			   sf::Color outline_normal_color = sf::Color::Black, sf::Color outline_hover_color = sf::Color::Black, sf::Color outline_pressed_color = sf::Color::Black,
			   short unsigned id = 0);

		Button(sf::Vector2f position, sf::Vector2f size, const std::string &textStr, unsigned character_size = 24)
			: Button(position, size, nullptr, textStr, character_size) {};

		Button() : Button({0.f, 0.f}, {50.f, 50.f}) {};

		virtual ~Button() = default;

		void updateEvents(sf::Event &sfEvent, const sf::Vector2f &mousePos) override;
		void update(const sf::Vector2f &mousePos) override;

		virtual sf::FloatRect getLocalBounds() const override;

		bool isPressed() const { return buttonState == ButtonState::PRESSED; }
		std::string getText() const { return text.getString(); }
		sf::Vector2f getSize() const { return shape.getSize(); }
		short unsigned getId() const { return id; };

		void setText(const std::string &textStr);
		void setId(unsigned short id) { this->id = id; }
		void setDisabled(bool disable);
		void onPressed(std::function<void()> callback) { onPressedCallback = std::move(callback); };

	protected:
		virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const override;

		bool isHovered(const sf::Vector2f &mousePos) const;

	private:
		ButtonState buttonState = ButtonState::NORMAL;
		short unsigned id = 0;
		bool buttonPressed = false;
		bool buttonReleased = false;

		sf::RectangleShape shape;
		sf::Text text;

		sf::Font *font = nullptr;
		static std::unique_ptr<sf::Font> defaultFont;

		std::function<void()> onPressedCallback = [] {};

		// Colors
		sf::Color textNormalColor;
		sf::Color textHoverColor;
		sf::Color textPressedColor;
		sf::Color textDisabledColor;

		sf::Color normalColor;
		sf::Color hoverColor;
		sf::Color pressedColor;
		sf::Color disabledColor;

		sf::Color outlineNormalColor;
		sf::Color outlineHoverColor;
		sf::Color outlinePressedColor;
		sf::Color outlineDisabledColor;

		// Helpers
		static sf::Font &loadFont(sf::Font *font);
	};
}