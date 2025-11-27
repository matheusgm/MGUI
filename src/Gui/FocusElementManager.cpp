#include "../stdafx.hpp"
#include "FocusElementManager.hpp"

void gui::FocusElementManager::setFocusElement(GuiElement *element)
{
    // 1. Desfoca o elemento atualmente focado (se houver e for diferente do novo)
    if (m_focusedElement != element && m_focusedElement != nullptr)
    {
        // Notifica o elemento anterior sobre a perda de foco
        m_focusedElement->onFocusChanged(false);
    }

    // 2. Define o novo elemento
    m_focusedElement = element;

    // 3. Notifica o novo elemento que ele está focado (se for válido)
    if (m_focusedElement != nullptr)
    {
        // Notifica o novo elemento sobre o ganho de foco
        m_focusedElement->onFocusChanged(true);
    }
}

void gui::FocusElementManager::clearFocus()
{
    if (m_focusedElement != nullptr)
    {
        // Notifica o elemento atual sobre a perda de foco
        m_focusedElement->onFocusChanged(false);
        cout << "Limpou" << endl;
        // Limpa o rastreador
        m_focusedElement = nullptr;
    }
}

gui::FocusElementManager &gui::FocusElementManager::getInstance()
{
    static FocusElementManager instance;
    return instance;
}