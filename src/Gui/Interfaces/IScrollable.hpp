#pragma once

namespace gui
{
    class IScrollable
    {
    public:
        virtual ~IScrollable() = default;

        // O 'delta' é o valor bruto do scroll (ex: +1 ou -1)
        virtual void scrollWheel(int delta) = 0;
    };
}