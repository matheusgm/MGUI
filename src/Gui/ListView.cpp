#include "../stdafx.hpp"
#include "ListView.hpp"

void gui::ListView::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
	states.transform *= this->getTransform();

	target.draw(m_background, states);

	sf::View oldView = target.getView();

	sf::Vector2f size = m_background.getSize();
	sf::Vector2u targetSize = target.getSize();

	sf::Vector2f globalPosition = states.transform.transformPoint({0.f, 0.f});

	sf::View renderView(sf::FloatRect({0.f, 0.f}, {size.x, size.y}));

	sf::FloatRect viewport({globalPosition.x / targetSize.x,
							globalPosition.y / targetSize.y},
						   {size.x / targetSize.x,
							size.y / targetSize.y});

	renderView.setViewport(viewport);
	target.setView(renderView);

	// 2. Preparação e Cálculo
	float itemHeight = m_adapter->getItemHeight();
	int firstVisibleItem = static_cast<int>(m_scrollOffset / itemHeight);
	int itemsToShow = static_cast<int>(m_viewport.size.y / itemHeight) + 1;

	// Limita o loop ao menor valor: itens visíveis OU o tamanho do buffer fixo (20)
	size_t loopLimit = std::min(static_cast<size_t>(itemsToShow), MAX_VIEWS_IN_BUFFER);

	// 3. FASE DE BIND E DESENHO
	for (size_t i = 0; i < loopLimit; ++i)
	{
		size_t dataIndex = firstVisibleItem + i;

		if (dataIndex >= m_adapter->getItemCount())
			continue;

		ListViewItem *currentView = m_viewBuffer[i].get();
		m_adapter->updateView(*currentView, dataIndex);

		sf::Vector2f itemPosition(0.f, (i * itemHeight) - (m_scrollOffset - (firstVisibleItem * itemHeight)));

		currentView->setPosition(itemPosition);
		target.draw(*currentView, sf::RenderStates::Default);
	}

	target.setView(oldView);
}

gui::ListView::ListView(const sf::Vector2f &position, const sf::Vector2f &size, std::unique_ptr<const IListViewAdapter> adapter)
	: GuiElement(position, size), m_adapter(std::move(adapter)), m_viewport(position, size)
{
	m_background.setSize(size);
	m_background.setFillColor(sf::Color::Yellow);
	m_background.setOutlineThickness(1.f);
	m_background.setOutlineColor(sf::Color::Black);

	for (size_t i = 0; i < MAX_VIEWS_IN_BUFFER; ++i)
		m_viewBuffer.push_back(m_adapter->createView());
}

void gui::ListView::updateEvents(sf::Event &sfEvent, const sf::Vector2f &mousePos)
{
	if (auto mouseEvent = sfEvent.getIf<sf::Event::MouseWheelScrolled>())
	{
		if (getGlobalBounds().contains(mousePos))
		{
			float itemHeight = m_adapter->getItemHeight();
			float scrollDelta = mouseEvent->delta * itemHeight;
			m_scrollOffset -= scrollDelta;

			float totalContentHeight = m_adapter->getItemCount() * itemHeight;
			float maxScroll = totalContentHeight - m_viewport.size.y;

			float finalMaxScroll = std::max(0.0f, maxScroll);

			m_scrollOffset = std::clamp(m_scrollOffset, 0.0f, finalMaxScroll);
		}
	}
}

void gui::ListView::update(const sf::Vector2f &mousePos)
{
}

sf::FloatRect gui::ListView::getGlobalBounds() const
{
	sf::FloatRect localBounds = m_background.getLocalBounds();
	return getTransform().transformRect(localBounds);
}
