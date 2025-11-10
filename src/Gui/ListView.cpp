#include "../stdafx.h"
#include "ListView.h"

gui::ListView::ListView(float x, float y, float width, float maxHeight)
	: BaseGui(sf::Vector2f(x, y), sf::Vector2f(width, maxHeight))
{
	backgroundShape.setFillColor(sf::Color::Yellow);
	backgroundShape.setOutlineThickness(1.f);
	backgroundShape.setOutlineColor(sf::Color::Black);

	initElements();
	initScroll();

	setSize(getWidth(), getHeight());
	setPosition(getLeft(), getTop());
}

gui::ListView::~ListView()
{
	delete scroll;
}

int gui::ListView::elementsInside()
{
	if (elements.empty())
		return 0;

	return static_cast<int>(floor(getHeight() / elementHeight));
}

int gui::ListView::elementsOutside()
{
	return totalElements() - elementsInside();
}
void gui::ListView::initElements()
{
	elements.push_back(new ListItem());
	Button *btn = new Button();
	btn->onPressed(
		[this]
		{
			cout << "oi" << endl;
		});
	elements.push_back(btn);
	elements.push_back(new ListItem());
	elements.push_back(new ListItem());
	elements.push_back(new ListItem());
	elements.push_back(new ListItem());
	elements.push_back(new ListItem(sf::Color::Blue));
	elements.push_back(new ListItem(sf::Color::White));
	elements.push_back(new ListItem(sf::Color::Magenta));
	elements.push_back(new ListItem(sf::Color::Green));
	elements.push_back(new ListItem());
	// elements.push_back(new ListItem());
	// elements.push_back(new ListItem());
	// elements.push_back(new ListItem());
	// elements.push_back(new ListItem());
}

void gui::ListView::initScroll()
{
	int elementosDentro = elementsInside();
	int elementosFora = elementsOutside();

	if (elementosFora > 0)
	{
		scroll = new Scroll(getRight() - scrollbarWidth, getTop(), scrollbarWidth, getHeight());
		scroll->setMaxValue(elementosFora);
		scroll->setIndicatorHeight((elementosDentro / (float)totalElements()) * scroll->getHeight());
		scroll->onValueChange(
			[this]
			{ setListItemPosition(scroll->getValue()); });
	}
}

void gui::ListView::addListItem(ListItem *item)
{
}

void gui::ListView::removeListItem(int pos)
{
}

void gui::ListView::setListItemPosition(int value)
{
	if (elements.size() == 0)
		return;

	int i = 0;
	for (auto &it : elements)
	{
		it->setPosition(getLeft(), getTop() + (i - value) * it->getSize().y);
		i++;
	}
}

int gui::ListView::totalElements()
{
	return elements.size();
}

void gui::ListView::setPosition(float x, float y)
{
	BaseGui::setPosition(x, y);

	backgroundShape.setPosition(getPosition());
	if (scroll)
		scroll->setPosition(getRight() - scroll->getWidth(), getTop());

	setListItemPosition(scroll ? scroll->getValue() : 0);
}

void gui::ListView::setSize(float width, float height)
{
	BaseGui::setSize(width, floor(height / elementHeight) * elementHeight);

	if (!elements.empty() && elementsOutside() <= 0)
		BaseGui::setSize(width, elementHeight * totalElements());

	backgroundShape.setSize(sf::Vector2f(getWidth() - scrollbarWidth, getHeight()));
	if (scroll)
		scroll->setSize(scroll->getWidth(), getHeight());

	for (auto &it : elements)
	{
		it->setSize(backgroundShape.getSize().x, elementHeight);
	}
}

void gui::ListView::updateEvents(sf::Event &sfEvent, const sf::Vector2f &mousePos)
{
	if (scroll)
	{
		scroll->updateEvents(sfEvent, mousePos);
		if (auto mouseEvent = sfEvent.getIf<sf::Event::MouseWheelScrolled>())
		{
			if (contains(mousePos))
			{
				scroll->scrollWheel(static_cast<int>(mouseEvent->delta));
				setListItemPosition(scroll->getValue());
			}
		}
	}
	for (auto &it : elements)
	{
		if (contains(it->getPosition()))
			it->updateEvents(sfEvent, mousePos);
	}
}

void gui::ListView::update(const sf::Vector2f &mousePos)
{
	if (scroll)
		scroll->update(mousePos);

	for (auto &it : elements)
	{
		if (contains(it->getPosition()))
			it->update(mousePos);
	}
}

void gui::ListView::render(sf::RenderTarget &target)
{
	target.draw(backgroundShape);

	sf::View oldView = target.getView();

	renderView.setViewport(sf::FloatRect({getLeft(), getTop()}, {getWidth(), getHeight()}));

	sf::FloatRect panelRect({getLeft() / target.getSize().x,
							 (getTop()) / target.getSize().y},
							{(getWidth()) / target.getSize().x,
							 (getHeight()) / target.getSize().y});

	renderView.setViewport(panelRect);

	target.setView(renderView);

	for (auto &it : elements)
	{
		if (contains(it->getPosition()) && it->getTop() < getBottom())
			it->render(target);
	}

	target.setView(oldView);

	if (scroll)
		scroll->render(target);
}
