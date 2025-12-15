#pragma once
#include <SFML/Window/Event.hpp>

namespace gui
{
    enum class KeyboardInputResult : uint8_t
    {
        Ignored, // O elemento não usou a tecla (o evento pode passar para outros)
        Handled, // O elemento usou a tecla (consumiu), mas quer manter o foco (ex: digitar 'A', setas)
        Submit,  // O elemento finalizou com sucesso (ex: Enter) -> Canvas deve tirar o foco
        Cancel   // O elemento cancelou (ex: Esc) -> Canvas deve tirar o foco e restaurar valor
    };

    class IKeyboardInput
    {
    public:
        virtual ~IKeyboardInput() = default;
        virtual KeyboardInputResult handleKeyboardInput(const sf::Event &sfEvent) = 0;
    };
}