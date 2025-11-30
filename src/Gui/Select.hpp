#pragma once
#include "Base/GuiElement.hpp"
namespace gui
{
    class Select : public GuiElement
    {
    public:
        Select(sf::Vector2f position, sf::Vector2f size);
        virtual ~Select() = default;

        virtual void handleMouseInput(sf::Event event, const sf::Vector2f &mousePos) override;
        virtual void update(sf::Time deltaTime) override;
        
        virtual sf::FloatRect getLocalBounds() const override;

    protected:
        virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const override;

    private:
        sf::RectangleShape shape;
    };
}
