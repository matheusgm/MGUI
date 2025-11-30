#pragma once
#include "Base/GuiElement.hpp"
#include "FocusElementManager.hpp"
#include "TextBox.hpp"

namespace gui
{
    class Canvas
    {
    public:
        Canvas();
        ~Canvas() = default;

        void update(sf::Time deltaTime);
        void handleEvent(sf::Event &sfEvent, const sf::Vector2f &mousePos);
        void handleResizeNotification();
        void handleContinuousMouseInput(const sf::Vector2f &mousePos);
        void draw(sf::RenderTarget &target);

        void addElement(std::unique_ptr<GuiElement> element);

        // Método para resetar o foco (chamado no onEnter do GameState, por exemplo)
        void resetFocus();

    private:
        std::vector<std::unique_ptr<GuiElement>> m_elements;

        FocusElementManager &m_focusManager;

        GuiElement *m_pressedElement = nullptr;
        bool m_ignoreNextTextEntered = false;

        GuiElement *findElementAt(const sf::Vector2f &mousePos);
        TextBox *findPreferredTextBoxToFocus();
        void checkFocusChange(const sf::Vector2f &mousePos);
    };
}
