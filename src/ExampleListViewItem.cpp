#include "stdafx.h"
#include "ExampleListViewItem.hpp"

ExampleListViewItem::ExampleListViewItem(const sf::Font &font, float height) : m_productName(font, "")
{
    m_background.setSize({800.0f, height});
    m_background.setFillColor(sf::Color(100, 100, 100));

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

void ExampleListViewItem::render(sf::RenderTarget &target)
{
    target.draw(m_background);
    target.draw(m_productName);
}

void ExampleListViewItem::setPosition(const float x, const float y)
{
    m_background.setPosition({x, y});
    m_productName.setPosition({x + 10.0f, y + m_background.getSize().y * 0.2f});
}
