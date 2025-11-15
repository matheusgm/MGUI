#include "Gui/ListViewItem.hpp"
#include "Model/Example.hpp"

class ExampleListViewItem : public gui::ListViewItem
{
public:
    ExampleListViewItem(const sf::Font &font, float height);

    void updateEvents(sf::Event &sfEvent, const sf::Vector2f &mousePos);
    void update(const sf::Vector2f &mousePos);
    virtual sf::FloatRect getLocalBounds() const override;

    void updateWithData(const Example &data, size_t index);

protected:
    virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const override;

private:
    sf::Text m_productName;
    sf::RectangleShape m_background;
};