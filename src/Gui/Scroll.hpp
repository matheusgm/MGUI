#pragma once
#include "Base/GuiElement.hpp"
#include "Button.hpp"

namespace gui
{
    class Scroll : public GuiElement
    {
    public:
        Scroll(sf::Vector2f position, sf::Vector2f size);
        virtual ~Scroll() = default;

        void updateEvents(sf::Event &sfEvent, const sf::Vector2f &mousePos) override;
        void update(const sf::Vector2f &mousePos) override;
        virtual sf::FloatRect getLocalBounds() const override;

        void scrollWheel(int delta);

        int getValue() const { return value; };
        int getMinValue() const { return minValue; };
        int getMaxValue() const { return maxValue; };
        void setIndicatorHeightRatio(float ratio);
        void setMinValue(int value);
        void setMaxValue(int value);
        void setValue(int value);
        void onValueChange(std::function<void()> callback) { onValueChangeCallback = std::move(callback); };

    protected:
        virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const override;

    private:
        sf::RectangleShape shape;
        sf::RectangleShape indicatorShape;

        std::unique_ptr<Button> buttonUp;
        std::unique_ptr<Button> buttonDown;

        std::function<void()> onValueChangeCallback = []() {};

        int minValue = 0;
        int maxValue = 100;
        int step = 1;
        int value = 0;
        float indicatorHeight = 50.f;
        float dragOffsetY = 0.f;
        bool indicatorPressed = false;

        void clampValue();
        void updateIndicatorPosition();
        void handleDrag(const sf::Vector2f &mousePos);
        float getButtonUpHeight() const;
        float getTrackHeight() const;
    };
}
