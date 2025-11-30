#pragma once

namespace gui
{
    class IFocusable
    {
    public:
        virtual ~IFocusable() = default;
        virtual void onFocusChanged(bool focused) = 0;
    };
}