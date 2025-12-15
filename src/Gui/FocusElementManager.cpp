#include "../stdafx.hpp"
#include "Base/GuiElement.hpp"
#include "FocusElementManager.hpp"
#include "Interfaces/IFocusable.hpp"
#include "Interfaces/IKeyboardInput.hpp"

void gui::FocusElementManager::setFocusElement(GuiElement *element)
{
    IFocusable *newFocusable = dynamic_cast<IFocusable *>(element);

    if (m_focusedElement != element && m_focusedElement != nullptr)
    {
        if (IFocusable *oldFocusable = dynamic_cast<IFocusable *>(m_focusedElement))
            oldFocusable->onFocusChanged(false);
    }

    if (newFocusable != nullptr)
    {
        m_focusedElement = element;
        newFocusable->onFocusChanged(true);
    }
    else
    {
        clearFocus();
    }
}

void gui::FocusElementManager::clearFocus()
{
    if (m_focusedElement != nullptr)
    {
        if (IFocusable *focusable = dynamic_cast<IFocusable *>(m_focusedElement))
        {
            focusable->onFocusChanged(false);
            cout << "Limpou" << endl;
        }

        m_focusedElement = nullptr;
    }
}

bool gui::FocusElementManager::handleKeyboardInput(const sf::Event &sfEvent)
{
    if (m_focusedElement != nullptr)
    {
        if (IKeyboardInput *keyboardInput = dynamic_cast<IKeyboardInput *>(m_focusedElement))
        {
            return keyboardInput->handleKeyboardInput(sfEvent);
        }
    }

    return false;
}