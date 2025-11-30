#include "../stdafx.hpp"
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
        // Notifica o elemento atual sobre a perda de foco
        if (IFocusable *focusable = dynamic_cast<IFocusable *>(m_focusedElement))
        {
            focusable->onFocusChanged(false);
            cout << "Limpou" << endl;
        }

        // Limpa o rastreador
        m_focusedElement = nullptr;
    }
}

bool gui::FocusElementManager::handleKeyboardInput(const sf::Event &sfEvent)
{
    if (m_focusedElement != nullptr)
    {
        // Verifica se o elemento focado implementa a interface de input de teclado
        if (IKeyboardInput *keyboardInput = dynamic_cast<IKeyboardInput *>(m_focusedElement))
        {
            // Delega o evento de teclado para o elemento focado
            return keyboardInput->handleKeyboardInput(sfEvent);
        }
        // Se o elemento estiver focado, mas não for IKeyboardInput (ex: um botão),
        // o evento é silenciosamente ignorado, o que é o comportamento desejado.
    }

    return false;
}

gui::FocusElementManager &gui::FocusElementManager::getInstance()
{
    static FocusElementManager instance;
    return instance;
}