#pragma once
#include "Base/GuiElement.hpp"
namespace gui
{
    class Select : public GuiElement
    {
    public:
        Select(sf::Vector2f position, sf::Vector2f size);
        virtual ~Select() = default;

        void updateEvents(sf::Event &sfEvent, const sf::Vector2f &mousePos) override;
        void update(const sf::Vector2f &mousePos) override;
        
        virtual sf::FloatRect getLocalBounds() const override;

    protected:
        virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const override;

    private:
        sf::RectangleShape shape;
    };
}
