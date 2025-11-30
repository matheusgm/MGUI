#pragma once

#include "Base/GuiElement.hpp"

namespace gui
{
    class FocusElementManager
    {
    public:
        static FocusElementManager &getInstance();

        void setFocusElement(GuiElement *element);
        void clearFocus();

        bool handleKeyboardInput(const sf::Event &sfEvent);

        void registerElement(GuiElement *element); // Se for usar TAB
        void unregisterElement(GuiElement *element); // Se for usar TAB

        GuiElement *getFocusedElement() const { return m_focusedElement; }

    private:
        FocusElementManager() = default;
        ~FocusElementManager() = default;
        FocusElementManager(const FocusElementManager &) = delete;
        FocusElementManager &operator=(const FocusElementManager &) = delete;

        GuiElement *m_focusedElement = nullptr;
        //std::vector<GuiElement *> m_focusableElements; // Se for usar TAB
    };
}