#pragma once

#include "Base/GuiElement.hpp"

namespace gui
{
    class FocusElementManager
    {
    public:
        static FocusElementManager& getInstance();

        void setFocusElement(GuiElement* element);
        void clearFocus();
        
        void registerElement(GuiElement* element);
        void unregisterElement(GuiElement* element);
        
        GuiElement* getFocusedElement() const { return m_focusedElement; }

    private:
        FocusElementManager() = default;
        ~FocusElementManager() = default;
        FocusElementManager(const FocusElementManager&) = delete;
        FocusElementManager& operator=(const FocusElementManager&) = delete;

        GuiElement *m_focusedElement = nullptr;
        std::vector<GuiElement*> m_focusableElements;

    };
}