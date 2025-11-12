#pragma once
#include "Base/GuiElement.hpp"
#include "Button.hpp"
#include "Model/DialogNode.hpp"

namespace gui
{
    class DialogBox : public GuiElement
    {
    private:
        sf::RectangleShape shape;
        sf::Text text;

        static std::unique_ptr<sf::Font> defaultFont;

        std::vector<gui::Button> buttons;
        Button closeButton;

        DialogType dialogType = DialogType::OK;

        std::function<void(const std::string &)> choiceCallback = [](const std::string &) {};

        // Helpers
        void updateText(const std::string &textStr);
        static sf::Font &loadFont();
        virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const override;

    public:
        DialogBox(sf::Vector2f position, sf::Vector2f size);
        virtual ~DialogBox() = default;

        void loadNode(const std::shared_ptr<DialogNode>& node);
        void setChoiceCallback(std::function<void(const std::string &)> callback) { choiceCallback = std::move(callback); };

        // Herdado por meio de GuiElement
        void updateEvents(sf::Event &sfEvent, const sf::Vector2f &mousePos) override;
        void update(const sf::Vector2f &mousePos) override;
        virtual sf::FloatRect getGlobalBounds() const override;
    };
}
