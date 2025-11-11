#pragma once

#include "./ListViewAdapterContract.hpp"
#include <vector>
#include <type_traits>

namespace gui
{
    template <typename T, typename U>
    class ListViewAdapter : public ListViewAdapterContract
    {
        static_assert(std::is_base_of_v<ListViewItem, U>, "O parâmetro U deve herdar de ListViewItem.");

    private:
        const std::vector<T> m_data;
        const sf::Font &m_font;
        const float m_height;

    public:
        ListViewAdapter(const std::vector<T> data, const sf::Font &font, float itemHeight)
            : m_data(data), m_font(font), m_height(itemHeight) {}

        float getItemHeight() const override { return m_height; }
        size_t getItemCount() const override { return m_data.size(); }

        std::unique_ptr<ListViewItem> createView() const override
        {
            return std::make_unique<U>(m_font, m_height);
        }

        void updateView(ListViewItem &view, size_t index) const override
        {
            if (index < m_data.size())
            {
                if (U *concreteView = dynamic_cast<U *>(&view))
                {
                    concreteView->updateWithData(m_data[index], index);
                }
            }
        }
    };
}