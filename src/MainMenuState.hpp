#pragma once

#include "SettingsState.hpp"
#include "DialogBoxState.hpp"
#include "Gui/Canvas.hpp"

class MainMenuState : public State
{
public:
    MainMenuState(StateData &state_data);
    virtual ~MainMenuState() = default;

    void updateKeyboardInput(sf::Event &event) override;
    void updateEvents(sf::Event &event) override;
    void onResizeWindow() override;
    void update(sf::Time deltaTime) override;
    void render(sf::RenderTarget &target) override;

private:
    sf::RectangleShape background;

    std::unique_ptr<gui::Canvas> m_guiCanvas;

    void initKeybinds();
    void initGui();
};
