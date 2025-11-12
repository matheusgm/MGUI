#include "Gui/ListViewItem.hpp"
#include "Model/Example.hpp"

class ExampleListViewItem : public gui::ListViewItem
{
private:
    sf::Text m_productName;
    sf::RectangleShape m_background;

    virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const override;

public:
    ExampleListViewItem(const sf::Font &font, float height);

    void updateWithData(const Example &data, size_t index);

    void updateEvents(sf::Event &sfEvent, const sf::Vector2f &mousePos);
    void update(const sf::Vector2f &mousePos);
    virtual sf::FloatRect getGlobalBounds() const override;
};