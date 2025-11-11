#pragma once

#include "Base/GuiElement.h"
#include "ListViewAdapterContract.hpp"

namespace gui
{
	class ListView : public GuiElement
	{
	private:
		std::unique_ptr<const ListViewAdapterContract> m_adapter;
		sf::RectangleShape m_background;
		sf::FloatRect m_viewport;
		float m_scrollOffset = 0.0f;

		std::vector<std::unique_ptr<ListViewItem>> m_viewBuffer;
		const size_t MAX_VIEWS_IN_BUFFER = 20;

	public:
		ListView(const sf::Vector2f &position, const sf::Vector2f &size, std::unique_ptr<const ListViewAdapterContract> adapter);
		~ListView() override = default;

		void updateEvents(sf::Event &sfEvent, const sf::Vector2f &mousePos) override;
		void update(const sf::Vector2f &mousePos) override;
		void render(sf::RenderTarget &target) override;
	};
}