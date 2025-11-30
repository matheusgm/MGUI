#pragma once

#include "Base/GuiElement.hpp"
#include "Interfaces/IPressable.hpp"

namespace gui
{
	class Slider : public GuiElement, public IPressable
	{
	public:
		Slider(sf::Vector2f position, sf::Vector2f size,
			   int min_value, int max_value, int default_value = 0, int step = 1,
			   sf::Color background_color = sf::Color(192, 192, 192, 220), sf::Color foreground_color = sf::Color(0, 100, 0, 220), sf::Color indicator_color = sf::Color(240, 240, 240, 255));
		virtual ~Slider() = default;

		virtual void update(sf::Time deltaTime) override;
		virtual void handleMouseInput(sf::Event event, const sf::Vector2f &mousePos) override;

		virtual sf::FloatRect getLocalBounds() const override;

		// Implementações de IPressable
		virtual void setPressedState(bool pressed, const sf::Vector2f &mousePos) override;
		virtual bool isBeingPressed() const override { return m_isPressed; }

		int getValue() const { return value; };

		void onValueChange(std::function<void()> callback) { onValueChangeCallback = std::move(callback); };

	protected:
		virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const override;

	private:
		sf::RectangleShape backgroundShape;
		sf::RectangleShape foregroundShape;
		sf::CircleShape indicatorShape;

		std::function<void()> onValueChangeCallback = [] {};

		bool m_isPressed = false;
		bool indicatorPressed = false;
		float dragOffsetX = 0.0f;

		int minValue = 0;
		int maxValue = 100;
		int step = 1;
		int value = 0;

		void updateIndicatorPosition();
		void handleDrag(const sf::Vector2f &mousePos);
	};
}