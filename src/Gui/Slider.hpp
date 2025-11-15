#pragma once

#include "Base/GuiElement.hpp"

namespace gui
{
	class Slider : public GuiElement
	{
	public:
		Slider(sf::Vector2f position, sf::Vector2f size,
			   int min_value, int max_value, int default_value = 0, int step = 1,
			   sf::Color background_color = sf::Color(192, 192, 192, 220), sf::Color foreground_color = sf::Color(0, 100, 0, 220), sf::Color indicator_color = sf::Color(240, 240, 240, 255));
		virtual ~Slider() = default;

		void updateEvents(sf::Event &sfEvent, const sf::Vector2f &mousePos) override;
		void update(const sf::Vector2f &mousePos) override;
		virtual sf::FloatRect getGlobalBounds() const override;
		virtual void setSize(const sf::Vector2f &newSize) override;

		int getValue() const { return value; };

		void onValueChange(std::function<void()> callback) { onValueChangeCallback = std::move(callback); };

	protected:
		virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const override;

	private:
		sf::RectangleShape backgroundShape;
		sf::RectangleShape foregroundShape;
		sf::CircleShape indicatorShape;

		std::function<void()> onValueChangeCallback = [] {};

		int minValue = 0;
		int maxValue = 100;
		int step = 1;
		int value = 0;
		float dragOffsetX = 0.f;
		bool indicatorPressed = false;

		void updateIndicatorPosition();
		void handleDrag(const sf::Vector2f &mousePos);
	};
}