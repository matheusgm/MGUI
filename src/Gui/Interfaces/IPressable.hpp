// IPressable.hpp
#pragma once

namespace gui
{
    class IPressable
    {
    public:
        virtual ~IPressable() = default;

        virtual bool isBeingPressed() const = 0;
        virtual void setPressedState(bool pressed, const sf::Vector2f& mousePos) = 0;
    };
}