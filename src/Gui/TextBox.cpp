#include "../stdafx.hpp"
#include "TextBox.hpp"
#include "FocusElementManager.hpp"

std::unique_ptr<sf::Font> gui::TextBox::defaultFont;

gui::TextBox::TextBox(sf::Vector2f position, sf::Vector2f size) : GuiElement(position), m_text(loadFont())
{
    // Shape
    m_background.setSize(size);
    m_background.setFillColor(sf::Color::White);
    m_background.setOutlineThickness(1.f);
    m_background.setOutlineColor(sf::Color::Black);

    // Text
    m_text.setFillColor(sf::Color::Black);
    m_text.setCharacterSize(14U);
    setText("");

    // Cursor
    m_cursor.setSize(sf::Vector2f(2.f, static_cast<float>(m_text.getCharacterSize())));
    m_cursor.setFillColor(sf::Color::Black);
    m_cursor.setOrigin({1.f, 0.f});

    alignText();
}

void gui::TextBox::handleMouseInput(sf::Event event, const sf::Vector2f &mousePos)
{
    if (m_isBeingSelected)
    {
        sf::Vector2f localMousePos = mapGlobalToLocal(mousePos);
        updateTextSelection(localMousePos);
    }
    else
    {
        // Lógica de Hover (mudar cor da borda quando o mouse passa)
    }
}

void gui::TextBox::update(sf::Time deltaTime)
{
    if (m_isFocused)
    {
        if (m_cursorClock.getElapsedTime().asSeconds() >= m_cursorBlinkRate)
        {
            m_showCursor = !m_showCursor;
            m_cursorClock.restart();
        }
    }
    else
    {
        m_showCursor = false;
        m_cursorClock.restart();
    }
}

sf::FloatRect gui::TextBox::getLocalBounds() const
{
    return m_background.getLocalBounds();
}

// void gui::TextBox::setSize(const sf::Vector2f &newSize)
// {
//     m_background.setSize(newSize);
//     alignText();
// }

void gui::TextBox::onFocusChanged(bool focused)
{
    m_isFocused = focused;

    if (focused)
    {
        m_background.setOutlineColor(sf::Color::Blue);
        m_showCursor = true;
        m_cursorClock.restart();
    }
    else
    {
        m_background.setOutlineColor(sf::Color::Black);
        m_showCursor = false;
    }

    updateVisualState();
}

void gui::TextBox::setPressedState(bool pressed, const sf::Vector2f &mousePos)
{
    m_isPressed = pressed;

    if (pressed)
    {
        sf::Vector2f localMousePos = mapGlobalToLocal(mousePos);

        positionCursor(localMousePos);

        m_isBeingSelected = true;
    }
    else
    {
        m_isBeingSelected = false;
    }
}

void gui::TextBox::handleKeyboardInput(const sf::Event &sfEvent)
{
    if (auto textEntered = sfEvent.getIf<sf::Event::TextEntered>())
        handleTextEnteredEvent(*textEntered);

    if (auto keyPressed = sfEvent.getIf<sf::Event::KeyPressed>())
    {
        if (keyPressed->code == cancellationKey)
        {
            if (onTextCanceled)
                onTextCanceled();

            gui::FocusElementManager::getInstance().clearFocus();
            return;
        }

        if (keyPressed->code == activationKey)
        {
            if (onTextSubmitted)
                onTextSubmitted(m_inputString);

            gui::FocusElementManager::getInstance().clearFocus();
            return;
        }
        // ... Lógica para setas (Left/Right), Home, End, etc.
    }
}

void gui::TextBox::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
    states.transform *= this->getTransform();

    target.draw(m_background, states);
    target.draw(m_text, states);

    if (m_isFocused && m_showCursor)
        target.draw(m_cursor, states);
}

void gui::TextBox::handleTextEnteredEvent(const sf::Event::TextEntered &textEvent)
{
    if (!m_isFocused)
        return;

    auto unicode = textEvent.unicode;

    if (unicode == 8) // Backspace
    {
        if (!m_inputString.empty())
            m_inputString.pop_back();
    }
    else if (unicode == 13 || unicode == 27) // Ignora Enter (13) e Escape (27)
    {
        return;
    }
    else if (unicode >= 32 && unicode <= 126) // Caracteres imprimíveis
    {
        if (m_inputString.size() < MAX_CHARS)
            m_inputString += static_cast<char>(unicode);
    }

    // Reseta o relógio e mostra o cursor após a entrada de texto
    m_showCursor = true;
    m_cursorClock.restart();
    updateText();
}

void gui::TextBox::updateVisualState()
{
}

void gui::TextBox::positionCursor(const sf::Vector2f &localMousePos)
{
}

void gui::TextBox::updateTextSelection(const sf::Vector2f &localMousePos)
{
}

void gui::TextBox::alignText()
{
    auto bounds = m_text.getLocalBounds();
    m_text.setOrigin({0.f, bounds.position.y + bounds.size.y / 2.f});

    m_text.setPosition({5.f, m_background.getSize().y / 2.f});

    sf::Vector2f cursorPosition;
    if (m_inputString.empty())
    {
        cursorPosition = m_text.getPosition();
    }
    else
    {
        cursorPosition = m_text.findCharacterPos(m_inputString.size());
    }

    m_cursor.setPosition({cursorPosition.x, m_background.getSize().y / 2.f - m_cursor.getSize().y / 2.f});
}

void gui::TextBox::updateText()
{
    m_text.setString(m_inputString);
    alignText();
}

void gui::TextBox::setText(const std::string &textStr)
{
    m_inputString = textStr;
    m_text.setString(textStr);
    alignText();
}

sf::Font &gui::TextBox::loadFont()
{
    if (!defaultFont)
    {
        defaultFont = std::make_unique<sf::Font>();
        if (!defaultFont->openFromFile("src/Fonts/MochiyPopPOne-Regular.ttf"))
            throw std::runtime_error("Failed to load default font!");
    }

    return *defaultFont;
}
