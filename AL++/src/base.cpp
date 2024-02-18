#include <AL++/base.hpp>

using namespace alpp;

std::string al::error_string(ALenum err)
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

void al::check_errors([[maybe_unused]] const char* func, [[maybe_unused]] const char* al_func)
{
    const ALenum err { ::alGetError() };
    HAL_ASSERT(err == AL_NO_ERROR, al_func, " returned ", error_string(err), " in ", func);
}

std::string alc::error_string(ALenum err)
{
    switch (err)
    {
    case ALC_NO_ERROR:
        return "ALC_NO_ERROR";
    case ALC_INVALID_DEVICE:
        return "ALC_INVALID_DEVICE";
    case ALC_INVALID_CONTEXT:
        return "ALC_INVALID_CONTEXT";
    case ALC_INVALID_ENUM:
        return "ALC_INVALID_ENUM";
    case ALC_INVALID_VALUE:
        return "ALC_INVALID_VALUE";
    case ALC_OUT_OF_MEMORY:
        return "ALC_OUT_OF_MEMORY";
    default:
        return std::string { "[unknown error " } + std::to_string(err) + ']';
    }
}

void alc::check_errors([[maybe_unused]] const char* func, [[maybe_unused]] const char* al_func, ALCdevice* dev)
{
    const ALenum err { ::alcGetError(dev) };
    HAL_ASSERT(err == ALC_NO_ERROR, al_func, " returned ", error_string(err), " in ", func);
}