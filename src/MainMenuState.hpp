#pragma once

#include "SettingsState.hpp"
#include "DialogBoxState.hpp"
#include "Gui/Button.hpp"

class MainMenuState : public State
{
public:
    MainMenuState(StateData &state_data);
    virtual ~MainMenuState() = default;

    void updateKeyboardInput(sf::Event &event) override;
    void updateEvents(sf::Event &event) override;
    void onResizeWindow() override;
    void updateGui() const;
    void update(float dt) override;
    void renderGui(sf::RenderTarget &target) const;
    void render(sf::RenderTarget &target) override;

private:
    sf::RectangleShape background;

    std::map<std::string, std::unique_ptr<gui::Button>> buttons;

    void initKeybinds();
    void initGui();
};
