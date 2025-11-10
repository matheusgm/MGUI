#pragma once

#include "Base/BaseGui.h"

namespace gui
{
	class Slider : BaseGui
	{
	private:
		sf::RectangleShape backgroundShape;
		sf::RectangleShape foregroundShape;

		sf::CircleShape indicatorShape;
		bool indicatorPressed;

		std::function<void()> onValueChangeCallback;

		int minValue;
		int maxValue;
		int step;

		int value;

	public:
		Slider(float x, float y, float width, float height,
			int min_value, int max_value, int default_value = 0, int step = 1,
			sf::Color background_color = sf::Color(192, 192, 192, 220), sf::Color foreground_color = sf::Color(0, 100, 0, 220), sf::Color indicator_color = sf::Color(240, 240, 240, 255)
		);
		virtual ~Slider() = default;;

		// Acessors
		const int getValue() const;

		// Modifier
		void setPosition(const float x, const float y) override;
		void setSize(const float width, const float height) override;
		void onValueChange(std::function<void()> callback);

		// Functions
		void updateIndicator();

		// Herdado por meio de BaseGui
		void updateEvents(sf::Event& sfEvent, const sf::Vector2f& mousePos) override;
		void update(const sf::Vector2f& mousePos) override;
		void render(sf::RenderTarget& target) override;
	};
}