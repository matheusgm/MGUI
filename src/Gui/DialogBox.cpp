#include "../stdafx.hpp"
#include "DialogBox.hpp"

std::unique_ptr<sf::Font> gui::DialogBox::defaultFont;

gui::DialogBox::DialogBox(sf::Vector2f position, sf::Vector2f size)
	: GuiElement(position),
	  text(loadFont())
{
	// Shape
	shape.setSize(size);
	shape.setFillColor(sf::Color::Red);
	shape.setOutlineThickness(1.f);
	shape.setOutlineColor(sf::Color::Black);

	// Text
	text.setFillColor(sf::Color::Black);
	text.setCharacterSize(24U);

	// Close Button
	closeButton = Button(
		{10.f, size.y - 35.f},
		{100.f, 25.f}, "Close", 16U);
}

// gui::DialogBox* gui::DialogBox::CreateOkDialog(float x, float y, float w, float h) {
//	std::map<std::string, gui::Button*> buttons;
//	buttons["OK"] = new gui::Button(w - 90.f, h - 32.f, 80.f, 24.f, "Ok", 20U);
//	return new DialogBox(x, y, w, h, buttons);
// }
//
// gui::DialogBox* gui::DialogBox::CreateYesNoDialog(float x, float y, float w, float h) {
//	std::map<std::string, gui::Button*> buttons;
//	buttons["YES"] = new gui::Button(w - 80.f - 100.f, h - 32.f, 80.f, 24.f, "Yes", 20U);
//	buttons["NO"] = new gui::Button(w - 90.f, h - 32.f, 80.f, 24.f, "No", 20U);
//	return new DialogBox(x, y, w, h, buttons);
// }
//
// gui::DialogBox* gui::DialogBox::CreateNextPrevDialog(float x, float y, float w, float h) {
//	std::map<std::string, gui::Button*> buttons;
//	buttons["PREVIOUS"] = new gui::Button(w - 80.f - 100.f, h - 32.f, 80.f, 24.f, "Previous", 20U);
//	buttons["NEXT"] = new gui::Button(w - 90.f, h - 32.f, 80.f, 24.f, "Next", 20U);
//	return new DialogBox(x, y, w, h, buttons);
// }

void gui::DialogBox::updateEvents(sf::Event &sfEvent, const sf::Vector2f &mousePos)
{
	sf::Vector2f scrollLocalMousePos = mapGlobalToLocal(mousePos);

	closeButton.updateEvents(sfEvent, scrollLocalMousePos);

	for (auto &btn : buttons)
		btn.updateEvents(sfEvent, scrollLocalMousePos);
}

void gui::DialogBox::update(const sf::Vector2f &mousePos)
{
	sf::Vector2f scrollLocalMousePos = mapGlobalToLocal(mousePos);

	closeButton.update(scrollLocalMousePos);

	for (auto &btn : buttons)
		btn.update(scrollLocalMousePos);
}

sf::FloatRect gui::DialogBox::getLocalBounds() const
{
	return shape.getLocalBounds();
}

void gui::DialogBox::loadNode(const std::shared_ptr<DialogNode> &node)
{

	dialogType = node->type;
	updateText(node->message);

	buttons.clear();
	buttons.reserve(node->options.size());

	constexpr float btnWidth = 100.f;
	constexpr float btnHeight = 30.f;
	constexpr float btnSpacing = 10.f;
	constexpr float margin = 10.f;

	float x = shape.getPosition().x + shape.getSize().x - margin;
	float y = shape.getPosition().y + shape.getSize().y - btnHeight - margin;

	if (node->type == DialogType::OK)
	{
		x -= btnWidth;
		gui::Button btn({x, y}, {btnWidth, btnHeight}, "Ok");
		btn.onPressed([this]()
					  { if (choiceCallback) choiceCallback("Ok"); });
		buttons.emplace_back(std::move(btn));
	}
	else
	{
		const float totalWidth = node->options.size() * (btnWidth + btnSpacing) - btnSpacing;
		x -= totalWidth;
		for (const auto &[label, _] : node->options)
		{
			gui::Button btn({x, y}, {btnWidth, btnHeight}, label);
			btn.onPressed([this, label]()
						  { if (choiceCallback) choiceCallback(label); });
			buttons.emplace_back(std::move(btn));
			x += btnWidth + btnSpacing;
		}
	}
}

void gui::DialogBox::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
	states.transform *= this->getTransform();

	target.draw(shape, states);
	target.draw(text, states);

	target.draw(closeButton, states);

	for (auto &btn : buttons)
		target.draw(btn, states);
}

void gui::DialogBox::updateText(const std::string &textStr)
{
	text.setString(textStr);
};

sf::Font &gui::DialogBox::loadFont()
{
	if (!defaultFont)
	{
		defaultFont = std::make_unique<sf::Font>();
		if (!defaultFont->openFromFile("src/Fonts/MochiyPopPOne-Regular.ttf"))
			throw std::runtime_error("Failed to load default font!");
	}

	return *defaultFont;
}