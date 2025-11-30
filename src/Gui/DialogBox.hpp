#pragma once
#include "Base/GuiElement.hpp"
#include "Button.hpp"
#include "Model/DialogNode.hpp"
#include "Interfaces/IPressable.hpp"

namespace gui
{
    class DialogBox : public GuiElement, public IPressable, public IFocusable
    {
    public:
        DialogBox(sf::Vector2f position, sf::Vector2f size);
        virtual ~DialogBox() = default;

        virtual void update(sf::Time deltaTime) override;
        virtual void handleMouseInput(sf::Event event, const sf::Vector2f &mousePos) override;

        virtual sf::FloatRect getLocalBounds() const override;

        virtual bool isBeingPressed() const override { return m_isPressed; }
        virtual void setPressedState(bool pressed, const sf::Vector2f &mousePos);

        virtual void onFocusChanged(bool focused);

        void loadNode(const std::shared_ptr<DialogNode> &node);
        void setChoiceCallback(std::function<void(DialogBox *self, const std::string &)> callback) { choiceCallback = std::move(callback); };

    protected:
        virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const override;

    private:
        sf::RectangleShape shape;
        sf::Text text;

        static std::unique_ptr<sf::Font> defaultFont;

        std::vector<gui::Button> buttons;
        std::unique_ptr<Button> closeButton;

        GuiElement *m_pressedChild = nullptr;
        bool m_isPressed = false;

        DialogType dialogType = DialogType::OK;

        std::function<void(DialogBox *self, const std::string &)> choiceCallback = [](DialogBox *self, const std::string &) {};

        // Helpers
        void updateText(const std::string &textStr);
        static sf::Font &loadFont();
        GuiElement *findChildAt(const sf::Vector2f &mousePos);
    };
}
