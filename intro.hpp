#pragma once

namespace halcyon
{
    class window;
    class image_loader;
    class ttf_engine;
    class mixer;
    class input_handler;
}  // namespace halcyon
namespace hq
{

    void intro(const halcyon::window& wnd, const halcyon::image_loader& imgl, const halcyon::ttf_engine& ttf, const halcyon::mixer& mix, const halcyon::input_handler& input) noexcept;
}  // namespace hq