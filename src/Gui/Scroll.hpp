#pragma once
#include "Base/GuiElement.hpp"
#include "Container.hpp"
#include "Interfaces/IScrollable.hpp"
#include "Button.hpp"

namespace gui
{
    class Scroll : public Container, public IScrollable
    {
    public:
        Scroll(sf::Vector2f position, sf::Vector2f size);
        virtual ~Scroll() = default;

        virtual void handleMouseInput(sf::Event event, const sf::Vector2f &mousePos) override;
        virtual void update(sf::Time deltaTime) override;
        virtual sf::FloatRect getLocalBounds() const override;

        // Implementações de IPressable
        virtual void setPressedState(bool pressed, const sf::Vector2f &mousePos) override;

        virtual void scrollWheel(int delta);

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
        gui::GuiElement *findChildAt(const sf::Vector2f &mousePos) override;

    private:
        sf::RectangleShape shape;
        sf::RectangleShape indicatorShape;

        std::unique_ptr<Button> buttonUp;
        std::unique_ptr<Button> buttonDown;

        std::function<void()> onValueChangeCallback = []() {};

        bool m_thumbPressed = false;

        int minValue = 0;
        int maxValue = 100;
        int step = 1;
        int value = 0;
        float indicatorHeight = 50.f;
        float dragOffsetY = 0.f;

        void clampValue();
        void updateIndicatorPosition();
        void handleDrag(const sf::Vector2f &mousePos);
        float getButtonUpHeight() const { return buttonUp->getSize().y; };
        float getTrackHeight() const { return shape.getSize().y; };
    };
}
