#include "../stdafx.hpp"
#include "Canvas.hpp"
#include "Interfaces/IScrollable.hpp"
#include "Interfaces/IPressable.hpp"
#include "Interfaces/IClickable.hpp"

gui::Canvas::Canvas() : m_focusManager(gui::FocusElementManager::getInstance())
{
    resetFocus();
}

void gui::Canvas::update(sf::Time deltaTime)
{
    for (auto &element : m_elements)
        element->update(deltaTime);
}

void gui::Canvas::handleEvent(sf::Event &sfEvent, const sf::Vector2f &mousePos)
{
    // 1. TRATAMENTO DE FOCO E TECLADO
    // O FocusManager deve ser o primeiro a lidar com input de teclado (IKeyboardInput)

    if (auto keyPressed = sfEvent.getIf<sf::Event::KeyPressed>())
    {
        if (m_focusManager.getFocusedElement() == nullptr)
        {
            gui::TextBox *textBoxToFocus = findPreferredTextBoxToFocus();
            if (textBoxToFocus && keyPressed->code == textBoxToFocus->getPrimaryFocusTargetKey())
            {
                m_focusManager.setFocusElement(textBoxToFocus);

                // ANULAR o evento TextEntered
                m_ignoreNextTextEntered = true;
                return;
            }
        }
    }

    if (sfEvent.getIf<sf::Event::TextEntered>() || sfEvent.getIf<sf::Event::KeyPressed>())
    {
        if (m_ignoreNextTextEntered)
        {
            m_ignoreNextTextEntered = false;
            return;
        }
        m_focusManager.handleKeyboardInput(sfEvent);
        return;
    }

    GuiElement *elementUnderMouse = findElementAt(mousePos);

    // 2. TRATAMENTO DA RODA DO MOUSE (IScrollable)
    if (auto mouseWheelScrolled = sfEvent.getIf<sf::Event::MouseWheelScrolled>())
    {
        if (IScrollable *scrollable = dynamic_cast<IScrollable *>(elementUnderMouse))
        {
            scrollable->scrollWheel(static_cast<int>(mouseWheelScrolled->delta));
            return;
        }
    }

    // 3. TRATAMENTO DE CLIQUES E PRESSIONAMENTO (IPressable / IClickable)
    if (sfEvent.getIf<sf::Event::MouseButtonPressed>())
    {
        m_focusManager.setFocusElement(elementUnderMouse);

        if (elementUnderMouse)
        {
            if (IPressable *pressable = dynamic_cast<IPressable *>(elementUnderMouse))
                pressable->setPressedState(true, mousePos);

            // Rastreia o elemento para o MouseReleased.
            m_pressedElement = elementUnderMouse;
        }
    }
    else if (sfEvent.getIf<sf::Event::MouseButtonReleased>())
    {
        if (m_pressedElement)
        {
            if (IPressable *pressable = dynamic_cast<IPressable *>(m_pressedElement))
                pressable->setPressedState(false, mousePos);

            if (IClickable *clickable = dynamic_cast<IClickable *>(elementUnderMouse))
            {
                if (elementUnderMouse == m_pressedElement)
                    clickable->executeClickAction();
            }
        }

        // 3. Limpa o rastreamento
        m_pressedElement = nullptr;
    }

    // 4. TRATAMENTO CONTÍNUO (MouseMoved)
    // O MouseMoved não dispara ações discretas, apenas prepara o estado de HOVER/DRAG.
    if (sfEvent.getIf<sf::Event::MouseMoved>())
    {
        // Este evento é o que dispara handleMouseInput() em todos os GuiElements
        // que precisam de HOVER.
        // No entanto, é mais eficiente chamá-lo no método Canvas::update()
        // ou após o loop de eventos, garantindo que o estado de HOVER/DRAG
        // seja atualizado em todos os frames, e não apenas quando o mouse se move.
        //
        // Se você não o mover para o update(), você pode chamá-lo aqui:
        // handleMouseInputForAllElements(mousePos);

        // for (auto &element : m_elements)
        //     element->handleMouseInput(sfEvent, mousePos);
        if (m_pressedElement)
            m_pressedElement->handleMouseInput(sfEvent, mousePos);
    }
}

void gui::Canvas::handleResizeNotification()
{
    for (auto &element : m_elements)
    {
        // Chama o método de resize em cada GuiElement
        // element->onWindowResize();
    }
}

void gui::Canvas::handleContinuousMouseInput(const sf::Vector2f &mousePos)
{
    if (m_pressedElement)
        return;

    GuiElement *topElement = findElementAt(mousePos);

    sf::Event mouseMovedEvent = sf::Event::MouseMoved();

    for (auto &element : m_elements)
    {
        if (element.get() == topElement)
            element->handleMouseInput(mouseMovedEvent, mousePos);
        else
            element->handleMouseInput(mouseMovedEvent, {-1.f, -1.f});
    }
}

void gui::Canvas::draw(sf::RenderTarget &target)
{
    for (auto &element : m_elements)
        target.draw(*element, sf::RenderStates::Default);
}

void gui::Canvas::addElement(std::unique_ptr<GuiElement> element)
{
    m_elements.push_back(std::move(element));

    // if (m_elements.back()->isFocusable())
    //     m_focusManager.registerElement(m_elements.back().get());
}

void gui::Canvas::resetFocus()
{
    m_focusManager.clearFocus();
}

void gui::Canvas::checkFocusChange(const sf::Vector2f &mousePos)
{
    GuiElement *newFocus = nullptr;

    for (auto it = m_elements.rbegin(); it != m_elements.rend(); ++it)
    {
        // if ((*it)->isFocusable() && (*it)->contains(mousePos))
        // {
        //     newFocus = it->get();
        //     break;
        // }
    }

    if (newFocus)
        m_focusManager.setFocusElement(newFocus);
    else
        m_focusManager.clearFocus();
}

gui::GuiElement *gui::Canvas::findElementAt(const sf::Vector2f &mousePos)
{
    // Itera sobre os elementos DE TRÁS PARA FRENTE (Z-order) para encontrar o elemento mais acima.
    for (auto it = m_elements.rbegin(); it != m_elements.rend(); ++it)
    {
        GuiElement *element = it->get();
        if (element->contains(mousePos))
            return element;
    }
    return nullptr;
}

gui::TextBox *gui::Canvas::findPreferredTextBoxToFocus()
{
    for (auto &element : m_elements)
    {
        if (gui::TextBox *textBox = dynamic_cast<gui::TextBox *>(element.get()))
        {
            if (textBox->isPrimaryFocusTarget())
                return textBox;
        }
    }

    return nullptr;
}
