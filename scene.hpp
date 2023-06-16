#pragma once

#include "camera.hpp"

namespace hal
{
    class window;
    class input_handler;
    class mixer;
}  // namespace hal

namespace hq
{
    class scene
    {
      public:

        scene(const hal::window& wnd, hal::input_handler& inp, hal::mixer& mxr) noexcept;

        void load_level(const char* path) noexcept;

      private:

        camera m_camera;

        const hal::window&  m_window;
        hal::input_handler& m_input;
    };
}  // namespace hq