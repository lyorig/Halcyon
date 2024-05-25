#include <halcyon/audio/device.hpp>

#include <halcyon/internal/helpers.hpp>

using namespace hal;

audio::spec::spec(i32 freq, audio::format fmt, u8 channels, u16 buffer_size_in_frames)
    : SDL_AudioSpec {
        .freq     = freq,
        .format   = static_cast<SDL_AudioFormat>(fmt),
        .channels = channels,
        .samples  = buffer_size_in_frames
    }
{
}

SDL_AudioSpec* audio::spec::get(pass_key<builder::device>)
{
    return this;
}

using adb = audio::builder::device;

adb::device(pass_key<proxy::audio>)
    : m_spec { 44100, format::i32, 2, 4096 }
    , m_name { nullptr }
    , m_allowedChanges { 0 }
    , m_capture { false }
{
}

adb& adb::name(std::string_view name)
{
    m_name = name.data();
    return *this;
}

adb& adb::capture()
{
    m_capture = true;
    return *this;
}

adb& adb::spec(const audio::spec& s)
{
    m_spec = s;
    return *this;
}

adb& adb::changes(std::initializer_list<change> vals)
{
    m_allowedChanges = detail::to_bitmask<int>(vals);
    return *this;
}

audio::device adb::operator()()
{
    return { m_name, m_capture, m_spec.get(pass_key<device> {}), nullptr, m_allowedChanges, pass_key<device> {} };
}

audio::device adb::operator()(audio::spec& obtained)
{
    return { m_name, m_capture, m_spec.get(pass_key<device> {}), obtained.get(pass_key<device> {}), m_allowedChanges, pass_key<device> {} };
}

audio::device::device(const char* name, bool capture, const SDL_AudioSpec* desired, SDL_AudioSpec* obtained, int allowed_changes, pass_key<builder::device>)
    : m_id { ::SDL_OpenAudioDevice(name, capture, desired, obtained, allowed_changes) }
{
    HAL_ASSERT(m_id != 0, debug::last_error());
}

audio::device::~device()
{
    ::SDL_CloseAudioDevice(m_id);
}

void audio::device::pause(bool p)
{
    ::SDL_PauseAudioDevice(m_id, p);
}

void audio::device::lock()
{
    ::SDL_LockAudioDevice(m_id);
}

void audio::device::unlock()
{
    ::SDL_UnlockAudioDevice(m_id);
}