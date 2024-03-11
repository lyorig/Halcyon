#pragma once

#include <halcyon/texture.hpp>
#include <memory>

namespace quest
{
    using frame_t     = lyo::u8;
    using frame_point = hal::point<frame_t>;

    enum class animation
    {
        idle,
        walk,
        slide,
        jump
    };

    class spritesheet
    {
    public:
        spritesheet(const hal::texture& tex, hal::pixel_point frame_size);

        void next();

        hal::pixel_rect get() const;

        frame_point      size() const;
        hal::pixel_point frame_size() const;

    private:
        std::unique_ptr<hal::pixel_point[]> m_buffer;

        // This is the same in every frame, so it gets cached.
        hal::pixel_point m_frameSize;
        frame_point      m_size, m_curr;
    };
}