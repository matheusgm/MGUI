#pragma once
#include "Base/BaseGui.h"
namespace gui
{
    class ListItem : public BaseGui
    {
    public:
        sf::RectangleShape shape;
        // sf::Text text;

        ListItem(sf::Color color = sf::Color::Red);
        virtual ~ListItem() = default;;

        void setPosition(float x, float y) override;
        void setSize(float x, float y) override;

        void updateEvents(sf::Event& sfEvent, const sf::Vector2f& mousePos) override;
        void update(const sf::Vector2f& mousePos) override;
        void render(sf::RenderTarget& target) override;
    };
}

