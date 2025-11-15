#pragma once
#include "Base/GuiElement.hpp"
#include "Button.hpp"
#include "Model/DialogNode.hpp"

namespace gui
{
    class DialogBox : public GuiElement
    {
    public:
        DialogBox(sf::Vector2f position, sf::Vector2f size);
        virtual ~DialogBox() = default;

        void updateEvents(sf::Event &sfEvent, const sf::Vector2f &mousePos) override;
        void update(const sf::Vector2f &mousePos) override;

        virtual sf::FloatRect getLocalBounds() const override;

        void loadNode(const std::shared_ptr<DialogNode> &node);
        void setChoiceCallback(std::function<void(const std::string &)> callback) { choiceCallback = std::move(callback); };

    protected:
        virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const override;

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
    };
}
