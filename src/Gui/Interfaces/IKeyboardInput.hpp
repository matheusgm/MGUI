#pragma once
#include <SFML/Window/Event.hpp>

namespace gui
{
    class IKeyboardInput
    {
    public:
        virtual ~IKeyboardInput() = default;
        virtual bool handleKeyboardInput(const sf::Event &sfEvent) = 0;
    };
}