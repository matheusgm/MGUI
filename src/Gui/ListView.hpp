#pragma once

#include "Base/GuiElement.hpp"
#include "Container.hpp"
#include "Interfaces/IListViewAdapter.hpp"
#include "Interfaces/IScrollable.hpp"
#include "Scroll.hpp"

namespace gui
{
	class ListView : public Container, public IScrollable
	{
	public:
		ListView(const sf::Vector2f &position, const sf::Vector2f &size, std::unique_ptr<const IListViewAdapter> adapter);
		~ListView() override = default;

		virtual void update(sf::Time deltaTime) override;
		virtual void handleMouseInput(sf::Event event, const sf::Vector2f &mousePos) override;

		virtual void setPressedState(bool pressed, const sf::Vector2f &mousePos) override;

		// Implementação da IScrollable
		virtual void scrollWheel(int delta) override;

		virtual sf::FloatRect getLocalBounds() const override;

	protected:
		virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const override;
		gui::GuiElement *findChildAt(const sf::Vector2f &mousePos) override;

	private:
		const size_t MAX_VIEWS_IN_BUFFER = 20;

		sf::RectangleShape m_background;
		sf::FloatRect m_viewport;
		float m_scrollOffset = 0.0f;
		int m_firstVisibleItem = 0;
		int m_itemsToShow = 0;

		std::unique_ptr<gui::Scroll> m_scrollBar;

		std::unique_ptr<const IListViewAdapter> m_adapter;
		std::vector<std::unique_ptr<ListViewItem>> m_viewBuffer;

		void calculateScrollLayout();
		void setupScrollBar();
	};
}