#include "../stdafx.h"
#include "ListView.h"

gui::ListView::ListView(float x, float y, float width, float maxHeight)
	: BaseGui(sf::Vector2f(x, y), sf::Vector2f(width, maxHeight))
{
	// Background Shape
	this->backgroundShape.setFillColor(sf::Color::Yellow);
	this->backgroundShape.setOutlineThickness(1.f);
	this->backgroundShape.setOutlineColor(sf::Color::Black);

	this->initElements();
	this->initScroll();

	this->setSize(getWidth(), getHeight());
	this->setPosition(getLeft(), getTop());
}

gui::ListView::~ListView()
{
	delete this->scroll;
}

int gui::ListView::elementsInside()
{
	if (this->elements.empty())
		return 0;

	return static_cast<int>(floor(getHeight() / this->elementHeight));
}

int gui::ListView::elementsOutside()
{
	return this->totalElements() - this->elementsInside();
}
void gui::ListView::initElements()
{
	this->elements.push_back(new ListItem());
	Button *btn = new Button();
	btn->onPressed(
		[this]
		{
			cout << "oi" << endl;
		});
	this->elements.push_back(btn);
	this->elements.push_back(new ListItem());
	this->elements.push_back(new ListItem());
	this->elements.push_back(new ListItem());
	this->elements.push_back(new ListItem());
	this->elements.push_back(new ListItem(sf::Color::Blue));
	this->elements.push_back(new ListItem(sf::Color::White));
	this->elements.push_back(new ListItem(sf::Color::Magenta));
	this->elements.push_back(new ListItem(sf::Color::Green));
	this->elements.push_back(new ListItem());
	// this->elements.push_back(new ListItem());
	// this->elements.push_back(new ListItem());
	// this->elements.push_back(new ListItem());
	// this->elements.push_back(new ListItem());
}

void gui::ListView::initScroll()
{
	int elementosDentro = this->elementsInside();
	int elementosFora = this->elementsOutside();

	if (elementosFora > 0)
	{
		this->scroll = new Scroll(this->getRight() - scrollbarWidth, this->getTop(), scrollbarWidth, this->getHeight());
		this->scroll->setMaxValue(elementosFora);
		this->scroll->setIndicatorHeight((elementosDentro / (float)this->totalElements()) * this->scroll->getHeight());
		this->scroll->onValueChange(
			[this]
			{ this->setListItemPosition(this->scroll->getValue()); });
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
	if (this->elements.size() == 0)
		return;

	int i = 0;
	for (auto &it : this->elements)
	{
		it->setPosition(this->getLeft(), this->getTop() + (i - value) * it->getSize().y);
		i++;
	}
}

int gui::ListView::totalElements()
{
	return this->elements.size();
}

void gui::ListView::setPosition(const float x, const float y)
{
	BaseGui::setPosition(x, y);

	this->backgroundShape.setPosition(this->getPosition());
	if (this->scroll)
		this->scroll->setPosition(this->getRight() - this->scroll->getWidth(), this->getTop());

	this->setListItemPosition(this->scroll ? this->scroll->getValue() : 0);
}

void gui::ListView::setSize(const float width, const float height)
{
	BaseGui::setSize(width, floor(height / this->elementHeight) * this->elementHeight);

	if (!this->elements.empty() && this->elementsOutside() <= 0)
		BaseGui::setSize(width, this->elementHeight * this->totalElements());

	this->backgroundShape.setSize(sf::Vector2f(this->getWidth() - scrollbarWidth, this->getHeight()));
	if (this->scroll)
		this->scroll->setSize(this->scroll->getWidth(), getHeight());

	for (auto &it : this->elements)
	{
		it->setSize(this->backgroundShape.getSize().x, this->elementHeight);
	}
}

void gui::ListView::updateEvents(sf::Event &sfEvent, const sf::Vector2f &mousePos)
{
	if (this->scroll)
	{
		this->scroll->updateEvents(sfEvent, mousePos);
		if (auto mouseEvent = sfEvent.getIf<sf::Event::MouseWheelScrolled>())
		{
			if (this->contains(mousePos))
			{
				this->scroll->scrollWheel(static_cast<int>(mouseEvent->delta));
				this->setListItemPosition(this->scroll->getValue());
			}
		}
	}
	for (auto &it : this->elements)
	{
		if (this->contains(it->getPosition()))
			it->updateEvents(sfEvent, mousePos);
	}
}

void gui::ListView::update(const sf::Vector2f &mousePos)
{
	if (this->scroll)
		this->scroll->update(mousePos);

	for (auto &it : this->elements)
	{
		if (this->contains(it->getPosition()))
			it->update(mousePos);
	}
}

void gui::ListView::render(sf::RenderTarget &target)
{
	target.draw(this->backgroundShape);

	sf::View oldView = target.getView();

	this->renderView.setViewport(sf::FloatRect({this->getLeft(), this->getTop()}, {this->getWidth(), this->getHeight()}));

	sf::FloatRect panelRect({this->getLeft() / target.getSize().x,
							 (this->getTop()) / target.getSize().y},
							{(this->getWidth()) / target.getSize().x,
							 (this->getHeight()) / target.getSize().y});

	this->renderView.setViewport(panelRect);

	target.setView(this->renderView);

	for (auto &it : this->elements)
	{
		if (this->contains(it->getPosition()) && it->getTop() < getBottom())
			it->render(target);
	}

	target.setView(oldView);

	if (this->scroll)
		this->scroll->render(target);
}
