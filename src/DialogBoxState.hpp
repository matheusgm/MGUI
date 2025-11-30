#pragma once

#include "State.hpp"
#include "Gui/Canvas.hpp"
#include "Gui/DialogTree.hpp"

class DialogBoxState : public State
{
public:
    DialogBoxState(StateData &state_data);
    virtual ~DialogBoxState() = default;

    void updateKeyboardInput(sf::Event &event) override;
    void updateEvents(sf::Event &event) override;
    void onResizeWindow() override;
    void update(sf::Time deltaTime) override;
    void render(sf::RenderTarget &target) override;

private:
    sf::RectangleShape background;

    std::unique_ptr<gui::Canvas> m_guiCanvas;

    std::unique_ptr<gui::DialogTree> dialogTree;

    // std::vector<sf::VideoMode> modes;

    void initVariables();
    void initKeybinds();
    void initGui();
};
