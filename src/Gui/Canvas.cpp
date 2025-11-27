#include "../stdafx.hpp"
#include "Canvas.hpp"

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
    GuiElement *focusedElement = m_focusManager.getFocusedElement();

    // if (focusedElement)
    //     focusedElement->handleKeyboardInput(sfEvent);

    // if(sfEvent.getIf<sf::Event::MouseButtonPressed>())
    //     checkFocusChange(mousePos); // Tenta mudar o foco ao clicar

    // for (auto &element : m_elements)
    //     element->handleMouseInput(mousePos);
}

void gui::Canvas::draw(sf::RenderTarget &target)
{
    // for (auto &element : m_elements)
    //     element->draw(target, sf::RenderStates::Default);
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