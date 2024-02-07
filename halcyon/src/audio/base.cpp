#include <halcyon/audio/base.hpp>

using namespace hal;

std::string audio::error_string(ALenum err)
{
    switch (err)
    {
    case AL_NO_ERROR:
        return "AL_NO_ERROR";
    case AL_INVALID_NAME:
        return "AL_INVALID_NAME";
    case AL_INVALID_ENUM:
        return "AL_INVALID_ENUM";
    case AL_INVALID_VALUE:
        return "AL_INVALID_VALUE";
    case AL_INVALID_OPERATION:
        return "AL_INVALID_OPERATION";
    case AL_OUT_OF_MEMORY:
        return "AL_OUT_OF_MEMORY";
    default:
        return std::string { "[unknown error " } + std::to_string(err) + ']';
    }
}

void audio::check_errors(const char* func, const char* al_func)
{
    const ALenum err { ::alGetError() };
    if (err != AL_NO_ERROR)
    {
        HAL_PANIC(audio::error_string(err), func, " in ", al_func);
    }
}