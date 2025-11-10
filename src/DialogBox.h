#pragma once
#include "Gui/Base/BaseGui.h"
#include "Gui/Button.h"
#include "DialogNode.h"

namespace gui
{
    class DialogBox : public BaseGui
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

    public:
        DialogBox(float x, float y, float width, float height);
        virtual ~DialogBox() = default;

        void loadNode(const std::shared_ptr<DialogNode>& node);
        void setChoiceCallback(std::function<void(const std::string &)> callback) { choiceCallback = std::move(callback); };

        // Modifier
        void setPosition(float x, float y) override;
        void setSize(float width, float height) override;

        // Herdado por meio de BaseGui
        void updateEvents(sf::Event &sfEvent, const sf::Vector2f &mousePos) override;
        void update(const sf::Vector2f &mousePos) override;
        void render(sf::RenderTarget &target) override;
    };
}
