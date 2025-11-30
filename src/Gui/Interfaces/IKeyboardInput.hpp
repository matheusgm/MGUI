#pragma once
#include <SFML/Window/Event.hpp>

namespace gui
{
    class IKeyboardInput
    {
    public:
        virtual ~IKeyboardInput() = default;
        virtual void handleKeyboardInput(const sf::Event &sfEvent) = 0;
    };
}