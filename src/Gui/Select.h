#pragma once
#include "Base/GuiElement.h"
namespace gui
{
    class Select :
        public GuiElement
    {
    private:
        sf::RectangleShape shape;

    public:
        Select(float x, float y, float width, float height);
        virtual ~Select() = default;;

        // Modifier
        void setPosition(const float x, const float y) override;
        void setSize(const float width, const float height) override;

        // Herdado por meio de GuiElement
        void updateEvents(sf::Event& sfEvent, const sf::Vector2f& mousePos) override;
        void update(const sf::Vector2f& mousePos) override;
        void render(sf::RenderTarget& target) override;
    };
}
