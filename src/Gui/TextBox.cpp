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

void gui::TextBox::updateEvents(sf::Event &sfEvent, const sf::Vector2f &mousePos)
{
    if (auto keyPressed = sfEvent.getIf<sf::Event::KeyPressed>())
    {
        if (m_isSelected)
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
                cout << "Submit Texto"<< endl;
                if (onTextSubmitted)
                    onTextSubmitted(m_inputString);

                gui::FocusElementManager::getInstance().clearFocus();
                return;
            }
        }
    }

    if (auto mouseClicked = sfEvent.getIf<sf::Event::MouseButtonPressed>())
    {
        if (mouseClicked->button == sf::Mouse::Button::Left)
        {
            // Mapeia a posição global do mouse para os limites globais do TextBox
            sf::FloatRect globalBounds = getTransform().transformRect(m_background.getLocalBounds());

            if (globalBounds.contains(mousePos))
            {
                setSelected(true); // Foca se o clique foi DENTRO
                return;
            }
            // Não precisamos de um 'else' aqui para desfocar,
            // pois o GuiManager lida com o clique fora de TODOS os elementos.
        }
    }

    handleTextEnteredEvent(sfEvent);
}

void gui::TextBox::update(sf::Time deltaTime)
{
    if (m_isSelected)
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

void gui::TextBox::setSize(const sf::Vector2f &newSize)
{
    m_background.setSize(newSize);
    alignText();
}

void gui::TextBox::onFocusChanged(bool focus)
{
    if (m_isSelected == focus)
        return;

    m_isSelected = focus;

    if (focus)
    {
        m_background.setOutlineColor(sf::Color::Blue);
        m_showCursor = true;
        m_cursorClock.restart();
    }else{
        m_background.setOutlineColor(sf::Color::Black);
    }

    updateText();
}

void gui::TextBox::setSelected(bool select)
{
    if (m_isSelected == select)
        return;

    if (select)
        gui::FocusElementManager::getInstance().setFocusElement(this);
}

void gui::TextBox::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
    states.transform *= this->getTransform();

    target.draw(m_background, states);
    target.draw(m_text, states);

    if (m_isSelected && m_showCursor)
        target.draw(m_cursor, states);
}

void gui::TextBox::handleTextEnteredEvent(sf::Event &event)
{
    if (m_isSelected)
    {
        if (auto textEntered = event.getIf<sf::Event::TextEntered>())
        {
            auto unicode = textEntered->unicode;
            const size_t MAX_CHARS = 50; // Limite de 50 caracteres

            if (unicode == 8) // Backspace
            {
                if (!m_inputString.empty())
                    m_inputString.pop_back();
            }
            else if (unicode == 13 || unicode == 27) // Ignora Enter (13) e Escape (27) no TextEntered
            {
                return;
            }
            else if (unicode >= 32 && unicode <= 126) // Caracteres imprimíveis ASCII (pode ser estendido para UTF-8)
            {
                if (m_inputString.size() < MAX_CHARS)
                    m_inputString += static_cast<char>(unicode);
            }

            // Reseta o relógio e mostra o cursor após a entrada de texto
            m_showCursor = true;
            m_cursorClock.restart();
            updateText();
        }
    }
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
