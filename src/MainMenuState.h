#pragma once

#include "SettingsState.h"
#include "DialogBoxState.h"
#include "Gui/Button.h"

class MainMenuState :
    public State
{
private: 
    // Variables
    sf::RectangleShape background;

    std::map<std::string, std::unique_ptr<gui::Button>> buttons;

    // Functions
    void initKeybinds();
    void initGui();

public:
    MainMenuState(StateData &state_data);
    virtual ~MainMenuState() = default;;

    void updateKeyboardInput(sf::Event& event) override;
    void updateEvents(sf::Event& event) override;
    void onResizeWindow() override;
    void update(float dt) override;
    void updateGui() const;
    void renderGui(sf::RenderTarget& target) const;
    void render(sf::RenderTarget& target) override;
};  

