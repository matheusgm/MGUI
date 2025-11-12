#include "stdafx.hpp"
#include "ExampleListViewItem.hpp"

void ExampleListViewItem::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
    states.transform *= this->getTransform();

    target.draw(m_background, states);
    target.draw(m_productName, states);
}

ExampleListViewItem::ExampleListViewItem(const sf::Font &font, float height) : m_productName(font, "")
{
    m_background.setSize({800.0f, height});
    m_background.setFillColor(sf::Color(100, 100, 100));
    m_background.setPosition({0.f, 0.f});

    m_productName.setCharacterSize(static_cast<unsigned int>(height * 0.2f));
    m_productName.setFillColor(sf::Color::White);
    m_productName.setPosition({10.0f, height * 0.2f});
}

void ExampleListViewItem::updateWithData(const Example &data, size_t index)
{
    std::string status = (data.estoque > 0) ? "" : " [ESGOTADO]";

    m_productName.setString("#" + std::to_string(index) + ": " + data.nome +
                            " - R$" + std::to_string(data.preco) + status);

    m_background.setFillColor(index % 2 == 0 ? sf::Color(70, 70, 70) : sf::Color(90, 90, 90));

    if (data.estoque == 0)
    {
        m_background.setFillColor(sf::Color(150, 50, 50));
    }
}

void ExampleListViewItem::updateEvents(sf::Event &sfEvent, const sf::Vector2f &mousePos)
{
}

void ExampleListViewItem::update(const sf::Vector2f &mousePos)
{
}

sf::FloatRect ExampleListViewItem::getGlobalBounds() const
{
    return m_background.getGlobalBounds();
}
