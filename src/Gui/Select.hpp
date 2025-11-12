#pragma once
#include "Base/GuiElement.hpp"
namespace gui
{
    class Select :
        public GuiElement
    {
    private:
        sf::RectangleShape shape;

        virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const override;

    public:
        Select(sf::Vector2f position, sf::Vector2f size);
        virtual ~Select() = default;

        // Herdado por meio de GuiElement
        void updateEvents(sf::Event& sfEvent, const sf::Vector2f& mousePos) override;
        void update(const sf::Vector2f& mousePos) override;
        virtual sf::FloatRect getGlobalBounds() const override;
    };
}
