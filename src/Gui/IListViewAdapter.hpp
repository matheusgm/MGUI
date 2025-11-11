#pragma once

#include "./ListViewItem.hpp"
#include <cstddef>
#include <memory>

namespace gui
{
    class IListViewAdapter
    {
    public:
        virtual ~IListViewAdapter() = default;

        virtual size_t getItemCount() const = 0;
        virtual float getItemHeight() const = 0;

        // Cria um novo componente visual (Usado no construtor do ListView)
        virtual std::unique_ptr<ListViewItem> createView() const = 0;

        // Atualiza um componente visual existente com novos dados (Usado no draw do ListView)
        virtual void updateView(ListViewItem &view, size_t index) const = 0;
    };
}