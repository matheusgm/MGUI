#pragma once

#include "State.hpp"
#include "Gui/Slider.hpp"
#include "Gui/Button.hpp"
#include "Gui/ListView.hpp"
#include "Gui/Select.hpp"
#include "Gui/Scroll.hpp"

class SettingsState : public State
{
public:
    SettingsState(StateData &state_data);
    virtual ~SettingsState() = default;

    void updateKeyboardInput(sf::Event &event) override;
    void updateEvents(sf::Event &event) override;
    void onResizeWindow() override;
    void updateGui(float dt) const;
    void update(float dt) override;
    void renderGui(sf::RenderTarget &target) const;
    void render(sf::RenderTarget &target) override;

private:
    sf::RectangleShape background;

    sf::Text soundText;
    sf::Text soundValue;

    std::map<std::string, std::unique_ptr<gui::Button>> buttons;
    std::unique_ptr<gui::Slider> soundSlider;
    std::unique_ptr<gui::ListView> listView;
    std::unique_ptr<gui::Select> select;
    std::unique_ptr<gui::Scroll> scroll;

    // std::vector<sf::VideoMode> modes;

    void initVariables();
    void initKeybinds();
    void initGui();
};
