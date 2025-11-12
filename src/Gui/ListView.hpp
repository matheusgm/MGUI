#pragma once

#include "Base/GuiElement.hpp"
#include "IListViewAdapter.hpp"
#include "Scroll.hpp"

namespace gui
{
	class ListView : public GuiElement
	{
	private:
		const size_t MAX_VIEWS_IN_BUFFER = 20;

		std::unique_ptr<const IListViewAdapter> m_adapter;
		sf::RectangleShape m_background;
		sf::FloatRect m_viewport;
		float m_scrollOffset = 0.0f;

		std::unique_ptr<gui::Scroll> m_scrollBar;

		int m_firstVisibleItem = 0;   
        int m_itemsToShow = 0;

		std::vector<std::unique_ptr<ListViewItem>> m_viewBuffer;

		void calculateScrollLayout();
        void setupScrollBar();

		virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const override;

	public:
		ListView(const sf::Vector2f &position, const sf::Vector2f &size, std::unique_ptr<const IListViewAdapter> adapter);
		~ListView() override = default;

		void updateEvents(sf::Event &sfEvent, const sf::Vector2f &mousePos) override;
		void update(const sf::Vector2f &mousePos) override;
		virtual sf::FloatRect getGlobalBounds() const override;
	};
}