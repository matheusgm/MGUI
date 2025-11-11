#include "Gui/ListViewItem.hpp"
#include "Model/Example.hpp"

class ExampleListViewItem : public gui::ListViewItem
{
private:
    sf::Text m_productName;
    sf::RectangleShape m_background;

public:
    ExampleListViewItem(const sf::Font &font, float height);

    void updateWithData(const Example &data, size_t index);

    void setPosition(const float x, const float y) override;
    void updateEvents(sf::Event &sfEvent, const sf::Vector2f &mousePos);
    void update(const sf::Vector2f &mousePos);
    void render(sf::RenderTarget &target);
};