#pragma once

#include "State.h"
#include "Gui/Slider.h"
#include "Gui/Button.h"
#include "Gui/ListView.h"
#include "Gui/Select.h"
#include "Gui/Scroll.h"

class SettingsState :
    public State 
{
private:
    // Variables
    sf::RectangleShape background;

    sf::Text soundText;
    sf::Text soundValue;

    std::map<std::string, std::unique_ptr<gui::Button>> buttons;
    std::unique_ptr<gui::Slider> soundSlider;
    std::unique_ptr<gui::ListView> listView;
    std::unique_ptr<gui::Select> select;
    std::unique_ptr<gui::Scroll> scroll;

    //std::vector<sf::VideoMode> modes;

    // Functions
    void initVariables();
    void initKeybinds();
    void initGui();

public:
    SettingsState(StateData &state_data);
    virtual ~SettingsState() = default;;

    void updateKeyboardInput(sf::Event& event) override;
    void updateEvents(sf::Event& event) override;
    void onResizeWindow() override;
    void update(float dt) override;
    void updateGui(float dt) const;
    void renderGui(sf::RenderTarget& target) const;
    void render(sf::RenderTarget& target) override;
};

