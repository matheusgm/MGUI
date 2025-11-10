#pragma once
#include "Gui/Base/BaseGui.h"
#include "Gui/Button.h"
#include "DialogNode.h"

namespace gui
{
    class DialogBox :
        public BaseGui
    {
    private:
        sf::RectangleShape shape;
        sf::Text text;
        std::string message;
        size_t currentIndex = 0;
        static std::unique_ptr<sf::Font> defaultFont;

        std::vector<gui::Button> buttons;
        Button closeButton;
        DialogType dialogType;
        std::function<void(const std::string&)> choiceCallback;

        void updateText();

    public:
        DialogBox(float x, float y, float width, float height, std::map<std::string, gui::Button*> buttons = {});
        virtual ~DialogBox() = default;;
        
        static sf::Font& loadFont();

        void loadNode(std::shared_ptr<DialogNode> node);
        void setChoiceCallback(std::function<void(const std::string&)> callback);

        // Modifier
        void setPosition(const float x, const float y) override;
        void setSize(const float width, const float height) override;

        // Herdado por meio de BaseGui
        void updateEvents(sf::Event& sfEvent, const sf::Vector2f& mousePos) override;
        void update(const sf::Vector2f& mousePos) override;
        void render(sf::RenderTarget& target) override;
    };
}

