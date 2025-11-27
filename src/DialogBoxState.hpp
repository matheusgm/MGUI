#pragma once

#include "State.hpp"
#include "Gui/DialogBox.hpp"
#include "Gui/DialogTree.hpp"
#include "Gui/Button.hpp"

class DialogBoxState : public State
{
public:
    DialogBoxState(StateData &state_data);
    virtual ~DialogBoxState() = default;

    void updateKeyboardInput(sf::Event &event) override;
    void updateEvents(sf::Event &event) override;
    void onResizeWindow() override;
    void updateGui(sf::Time deltaTime) const;
    void update(sf::Time deltaTime) override;
    void renderGui(sf::RenderTarget &target) const;
    void render(sf::RenderTarget &target) override;

private:
    sf::RectangleShape background;

    std::map<std::string, std::unique_ptr<gui::Button>> buttons;

    // Dialog Box
    std::unique_ptr<gui::DialogTree> dialogTree;
    std::unique_ptr<gui::DialogBox> dialogBox;

    // std::vector<sf::VideoMode> modes;

    void initVariables();
    void initKeybinds();
    void initGui();
};
