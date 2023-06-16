#pragma once

namespace hal
{
    class window;
    class image_loader;
    class ttf_engine;
    class mixer;
    class input_handler;
}  // namespace hal
namespace hq
{
    void intro(const hal::window& wnd, const hal::image_loader& imgl, const hal::ttf_engine& ttf, const hal::mixer& mix, const hal::input_handler& input) noexcept;
}  // namespace hq