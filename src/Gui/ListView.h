#pragma once

#include "Base/BaseGui.h"
#include "Scroll.h"
#include "ListItem.h"

namespace gui
{
	class ListView : BaseGui
	{
	private:
		sf::View renderView;
		sf::RectangleShape backgroundShape;

		Scroll *scroll;
		float scrollbarWidth = 10.f;

		vector<BaseGui *> elements;
		float elementHeight = 50.f;

		int elementsInside();
		int elementsOutside();

	public:
		ListView(float x, float y, float width, float height);
		virtual ~ListView();

		void initElements();
		void initScroll();

		void addListItem(ListItem *item);
		void removeListItem(int pos);
		void setListItemPosition(int value);

		int totalElements();

		// Modifier
		void setPosition(float x, float y) override;
		void setSize(float width, float height) override;

		// Herdado por meio de BaseGui
		void updateEvents(sf::Event &sfEvent, const sf::Vector2f &mousePos) override;
		void update(const sf::Vector2f &mousePos) override;
		void render(sf::RenderTarget &target) override;
	};
}
