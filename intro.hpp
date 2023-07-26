#pragma once

namespace hal
{
    class window;
    class image_loader;
    class font_loader;
    class mixer;
    class input_handler;
}  // namespace hal
namespace game
{
    void intro(hal::window& wnd, const hal::image_loader& imgl, const hal::font_loader& fl, const hal::mixer& mix, hal::input_handler& input) noexcept;
}  // namespace game