#pragma once

#include "Base/GuiElement.hpp"
#include "Interfaces/IFocusable.hpp"
#include "Interfaces/IKeyboardInput.hpp"
#include "Interfaces/IPressable.hpp"

namespace gui
{
    class TextBox : public GuiElement, public IFocusable, public IKeyboardInput, public IPressable
    {
    public:
        TextBox(sf::Vector2f position, sf::Vector2f size);
        virtual ~TextBox() = default;

        virtual void handleMouseInput(sf::Event event, const sf::Vector2f &mousePos) override;
        virtual void update(sf::Time deltaTime) override;
        virtual sf::FloatRect getLocalBounds() const override;

        virtual void onFocusChanged(bool focused);

        virtual bool isBeingPressed() const override { return m_isPressed; }
        virtual void setPressedState(bool pressed, const sf::Vector2f &mousePos) override;

        virtual KeyboardInputResult handleKeyboardInput(const sf::Event &sfEvent) override;

        std::string getText() const { return m_inputString; };

        void setAsPrimaryFocusTarget(bool isPrimary, sf::Keyboard::Key key = sf::Keyboard::Key::Enter)
        {
            m_isPrimaryFocusTarget = isPrimary;
            m_primaryFocusTargetKey = key;
        }
        bool isPrimaryFocusTarget() const { return m_isPrimaryFocusTarget; }
        sf::Keyboard::Key getPrimaryFocusTargetKey() const { return m_primaryFocusTargetKey; }

        void setOnTextSubmittedCallback(std::function<void(const std::string &)> callback) { onTextSubmitted = std::move(callback); };
        void setOnTextCanceledCallback(std::function<void()> callback) { onTextCanceled = std::move(callback); };

    protected:
        virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const override;

    private:
        sf::RectangleShape m_background;
        sf::Text m_text;
        std::string m_inputString = "";

        const size_t MAX_CHARS = 50;

        bool m_isPrimaryFocusTarget = false;
        sf::Keyboard::Key m_primaryFocusTargetKey = sf::Keyboard::Key::Enter;

        // Cursor
        sf::RectangleShape m_cursor;
        sf::Clock m_cursorClock;
        float m_cursorBlinkRate = 0.5f;
        bool m_showCursor = false;
        std::size_t m_cursorIndex = 0;

        bool m_isFocused = false;
        bool m_isPressed = false;
        bool m_isBeingSelected = false;

        sf::Keyboard::Key activationKey = sf::Keyboard::Key::Enter;
        sf::Keyboard::Key cancellationKey = sf::Keyboard::Key::Escape;

        std::function<void(const std::string &)> onTextSubmitted = [](const std::string &) {};
        std::function<void()> onTextCanceled = [] {};

        static std::unique_ptr<sf::Font> defaultFont;

        void handleTextEnteredEvent(const sf::Event::TextEntered &textEvent);
        void updateCursorVisualPosition();
        void positionCursor(const sf::Vector2f &localMousePos);
        void updateTextSelection(const sf::Vector2f &localMousePos);
        void alignText();
        void updateText();
        void setText(const std::string &textStr);

        static sf::Font &loadFont();
    };
}