#pragma once

#include "Base/GuiElement.hpp"
#include "Interfaces/IClickable.hpp"
#include "Interfaces/IPressable.hpp"
#include "Interfaces/IFocusable.hpp"
#include "Interfaces/IKeyboardInput.hpp"

namespace gui
{
	enum class ButtonState : uint8_t
	{
		NORMAL,
		HOVER,
		PRESSED,
		FOCUSED,
		DISABLED
	};

	class Button : public GuiElement, public IClickable, public IPressable
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

		virtual void update(sf::Time deltaTime) override;
		virtual void handleMouseInput(sf::Event event, const sf::Vector2f &mousePos) override;

		virtual sf::FloatRect getLocalBounds() const override;

		// --- Implementação de IClickable ---
		virtual void executeClickAction() override;

		// --- Implementação de IPressable ---
		virtual bool isBeingPressed() const override { return m_isPressed; }
		virtual void setPressedState(bool pressed, const sf::Vector2f& mousePos) override;

		std::string getText() const { return text.getString(); }
		sf::Vector2f getSize() const { return shape.getSize(); }

		void setText(const std::string &textStr);
		void onPressed(std::function<void()> callback) { onPressedCallback = std::move(callback); };
		void setDisabled(bool disable);

	protected:
		virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const override;
		bool isHovered(const sf::Vector2f &mousePos) { return contains(mousePos); }
		void centerText();
		void updateVisualState();

	private:
		ButtonState buttonState = ButtonState::NORMAL;
		bool m_isFocused = false;
		bool m_isPressed = false;
		bool m_isDisabled = false;

		sf::RectangleShape shape;
		sf::Text text;
		sf::Font *font = nullptr;
		static std::unique_ptr<sf::Font> defaultFont;

		std::function<void()> onPressedCallback = [] {};

		// Colors
		sf::Color textNormalColor, textHoverColor, textPressedColor, textDisabledColor;
		sf::Color normalColor, hoverColor, pressedColor, disabledColor;
		sf::Color outlineNormalColor, outlineHoverColor, outlinePressedColor, outlineDisabledColor;

		// Helpers
		static sf::Font &loadFont(sf::Font *font);
	};
}