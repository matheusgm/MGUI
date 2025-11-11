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

    void render(sf::RenderTarget &target) override;

    void setPosition(const float x, const float y) override;
};