#pragma once

#include <string_view>

#define HAL_DETAIL_DEFINE_TEMPLATED_OSTREAM                   \
    template <typename T>                                     \
        requires requires(const T& x) { hal::to_string(x); }  \
    std::ostream& operator<<(std::ostream& str, const T& obj) \
    {                                                         \
        return str << hal::to_string(obj);                    \
    }

#define HAL_DETAIL_DEFINE_TEMPLATED_OSTREAM_NAMESPACE(nms) \
    namespace nms                                          \
    {                                                      \
        HAL_DETAIL_DEFINE_TEMPLATED_OSTREAM                \
    }

namespace hal
{
    std::string_view to_string();

    HAL_DETAIL_DEFINE_TEMPLATED_OSTREAM

    HAL_DETAIL_DEFINE_TEMPLATED_OSTREAM_NAMESPACE(pixel)
    HAL_DETAIL_DEFINE_TEMPLATED_OSTREAM_NAMESPACE(event)
    HAL_DETAIL_DEFINE_TEMPLATED_OSTREAM_NAMESPACE(keyboard)
    HAL_DETAIL_DEFINE_TEMPLATED_OSTREAM_NAMESPACE(mouse)
    HAL_DETAIL_DEFINE_TEMPLATED_OSTREAM_NAMESPACE(audio)
}