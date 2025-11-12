#pragma once
#include "Base/GuiElement.hpp"
#include "Button.hpp"

namespace gui
{
    class Scroll : public GuiElement
    {
    private:
        sf::RectangleShape shape;

        std::unique_ptr<Button> buttonUp;
        std::unique_ptr<Button> buttonDown;

        sf::RectangleShape indicatorShape;
        bool indicatorPressed = false;

        std::function<void()> onValueChangeCallback = []() {};

        int minValue = 0;
        int maxValue = 100;
        int step = 1;
        int value = 0;
        float indicatorHeight = 50.f;
        float dragOffsetY = 0.f;

        void clampValue();
        void updateIndicatorPosition();
        void handleDrag(const sf::Vector2f &mousePos);
        virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const override;

    public:
        Scroll(sf::Vector2f position, sf::Vector2f size);
        virtual ~Scroll() = default;

        void scrollWheel(int delta);
        int getValue() const { return value; };

        void setMinValue(int value)
        {
            minValue = value;
            clampValue();
            updateIndicatorPosition();
        }
        void setMaxValue(int value)
        {
            maxValue = value;
            clampValue();
            updateIndicatorPosition();
        }
        int getMinValue() const { return minValue; };
        int getMaxValue() const { return maxValue; };
        void setIndicatorHeight(float height) { indicatorHeight = height; }
        void onValueChange(std::function<void()> callback) { onValueChangeCallback = std::move(callback); };

        // Herdado por meio de GuiElement
        void updateEvents(sf::Event &sfEvent, const sf::Vector2f &mousePos) override;
        void update(const sf::Vector2f &mousePos) override;
        virtual sf::FloatRect getGlobalBounds() const override;
    };
}
