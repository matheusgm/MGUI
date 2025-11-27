#pragma once

#include "Base/GuiElement.hpp"

namespace gui
{
    class TextBox : public GuiElement
    {
    public:
        TextBox(sf::Vector2f position, sf::Vector2f size);
        virtual ~TextBox() = default;

        void updateEvents(sf::Event &sfEvent, const sf::Vector2f &mousePos) override;
        void update(sf::Time deltaTime) override;
        virtual sf::FloatRect getLocalBounds() const override;
        virtual void setSize(const sf::Vector2f &newSize) override;
        virtual void onFocusChanged(bool focus) override;

        bool isSelected() const { return m_isSelected; };
        void setSelected(bool select);
        std::string getText() const { return m_inputString; };

        void setOnTextSubmittedCallback(std::function<void(const std::string &)> callback) { onTextSubmitted = std::move(callback); };
        void setOnTextCanceledCallback(std::function<void()> callback) { onTextCanceled = std::move(callback); };

    protected:
        virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const override;

    private:
        sf::RectangleShape m_background;
        sf::Text m_text;
        std::string m_inputString = "";

        const size_t MAX_CHARS = 50;

        // Cursor
        sf::RectangleShape m_cursor;
        sf::Clock m_cursorClock;
        float m_cursorBlinkRate = 0.5f;
        bool m_showCursor = false;

        bool m_isSelected = false;

        sf::Keyboard::Key activationKey = sf::Keyboard::Key::Enter;
        sf::Keyboard::Key cancellationKey = sf::Keyboard::Key::Escape;

        std::function<void(const std::string &)> onTextSubmitted = [](const std::string &) {};
        std::function<void()> onTextCanceled = [] {};

        static std::unique_ptr<sf::Font> defaultFont;

        void handleTextEnteredEvent(sf::Event &event);
        void alignText();
        void updateText();
        void setText(const std::string &textStr);

        static sf::Font &loadFont();
    };
}