#include <halcyon/audio/decoder.hpp>

using namespace hal::audio;

struct wave_header
{
    char riff[4] {};

    std::int32_t total_size;

    char fth[4] {}; // File type header.
    char fcm[4] {}; // Format chunk marker.

    std::int32_t format_length;
    std::int16_t format;
    std::int16_t channels;
    std::int32_t sample_rate;
    std::int32_t sample_rate_calc;
    std::int16_t weird_calc;
    std::int16_t bits_per_sample;

    char data[4];

    std::int32_t data_size;
};

format wav2fmt(std::int16_t fmt)
{
    switch (fmt)
    {
    case 1:
        return mono8;
    case 2:
        return stereo8;
    case 4:
        return stereo16;
    default:
        std::unreachable();
    }
}

buffer decoder::wav(const std::string_view& path)
{
    wave_header hdr;

    std::ifstream i { path };
    HAL_ASSERT(i.is_open(), "Couldn't open file");

    i.read(hdr.riff, 4);
    HAL_ASSERT(std::strncmp(hdr.riff, "RIFF", 4) == 0, "RIFF not found");

    i.read((char*)&hdr.total_size, sizeof(hdr.total_size));

    i.read(hdr.fth, 4);
    HAL_ASSERT(std::strncmp(hdr.fth, "WAVE", 4) == 0, "WAVE not found");

    i.read(hdr.fcm, 4);
    HAL_ASSERT(std::strncmp(hdr.fcm, "fmt", 3) == 0, "fmt not found");

    i.read((char*)&hdr.format_length, sizeof(hdr.format_length));
    i.read((char*)&hdr.format, sizeof(hdr.format));
    i.read((char*)&hdr.channels, sizeof(hdr.channels));
    i.read((char*)&hdr.sample_rate, sizeof(hdr.sample_rate));
    i.read((char*)&hdr.sample_rate_calc, sizeof(hdr.sample_rate_calc));
    i.read((char*)&hdr.weird_calc, sizeof(hdr.weird_calc));
    i.read((char*)&hdr.bits_per_sample, sizeof(hdr.bits_per_sample));

    i.read(hdr.data, 4);
    HAL_ASSERT(std::strncmp(hdr.data, "data", 4) == 0, "data not found");

    i.read((char*)&hdr.data_size, sizeof(hdr.data_size));

    HAL_PRINT("Size: ", hdr.total_size, ", ", hdr.channels, " channel(s), sample rate ", hdr.sample_rate, " data size ", hdr.data_size);

    std::unique_ptr<char[]> alloc { std::make_unique<char[]>(hdr.data_size) };
    i.read(alloc.get(), hdr.data_size);

    return { { alloc.get(), std::size_t(hdr.data_size) }, wav2fmt(hdr.format), hdr.sample_rate, {} };
}
