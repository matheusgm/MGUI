#pragma once

#include "State.hpp"
#include "Gui/Canvas.hpp"
#include "Gui/Slider.hpp"

class SettingsState : public State
{
public:
    SettingsState(StateData &state_data);
    virtual ~SettingsState() = default;

    void updateKeyboardInput(sf::Event &event) override;
    void updateEvents(sf::Event &event) override;
    void onResizeWindow() override;
    void update(sf::Time deltaTime) override;
    void render(sf::RenderTarget &target) override;

private:
    sf::RectangleShape background;

    sf::Text soundText;
    sf::Text soundValue;
    gui::Slider* m_soundSlider = nullptr;

    std::unique_ptr<gui::Canvas> m_guiCanvas;

    sf::Text debugLinePositionText;

    // std::vector<sf::VideoMode> modes;

    void initVariables();
    void initKeybinds();
    void initGui();
};
