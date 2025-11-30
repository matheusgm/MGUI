#pragma once
#include "Base/GuiElement.hpp"
#include "Interfaces/IPressable.hpp"

namespace gui
{
    class Container : public GuiElement, public IPressable
    {
    public:
        Container(sf::Vector2f position);
        virtual ~Container() = default;

        virtual bool isBeingPressed() const override { return m_isPressed; };
        virtual void setPressedState(bool pressed, const sf::Vector2f &mousePos) override;

    protected:
        GuiElement *m_pressedChild = nullptr;
        bool m_isPressed = false;

        bool handleChildPress(bool pressed, const sf::Vector2f &localMousePos);
        virtual GuiElement *findChildAt(const sf::Vector2f &mousePos) = 0;
    };
}