#include "../stdafx.hpp"
#include "Container.hpp"
#include "Interfaces/IClickable.hpp"

gui::Container::Container(sf::Vector2f position) : GuiElement(position)
{
}

void gui::Container::setPressedState(bool pressed, const sf::Vector2f &mousePos)
{
    sf::Vector2f localPos = mapGlobalToLocal(mousePos);

    if (!handleChildPress(pressed, localPos))
    {
        if (!pressed)
            m_isPressed = false;
        // Se quiser que containers vazios sejam pressionáveis, adicione lógica aqui.
    }
}

bool gui::Container::handleChildPress(bool pressed, const sf::Vector2f &localMousePos)
{
    GuiElement *childUnderMouse = findChildAt(localMousePos);

    if (pressed)
    {
        if (childUnderMouse)
        {
            if (IPressable *pressableChild = dynamic_cast<IPressable *>(childUnderMouse))
            {
                pressableChild->setPressedState(true, localMousePos);
                m_pressedChild = childUnderMouse;
                return true; // Filho consumiu o clique
            }
        }
    }
    else
    {
        // Lógica de Release genérica
        if (m_pressedChild)
        {
            // Executa click se soltou no mesmo filho
            if (childUnderMouse == m_pressedChild)
            {
                if (IClickable *clickableChild = dynamic_cast<IClickable *>(m_pressedChild))
                    clickableChild->executeClickAction();
            }

            // Despressiona o filho
            if (IPressable *pressableChild = dynamic_cast<IPressable *>(m_pressedChild))
                pressableChild->setPressedState(false, localMousePos);

            m_pressedChild = nullptr;
            return true;
        }
    }

    return false;
}
