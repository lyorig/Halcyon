#define MINIMP3_IMPLEMENTATION

#include <AL++/decoder.hpp>
#include <AL++/external/minimp3_ex.h>
#include <fstream>
#include <lyo/types.hpp>
#include <vector>

using namespace alpp;

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

format get_format(lyo::u8 channels, lyo::u16 bits_per_sample)
{
    constexpr std::array l { mono8, mono16, stereo8, stereo16 };
    return l[2 * (channels - 1) + (bits_per_sample / 16)];
}

buffer decoder::wav(const std::filesystem::path& path)
{
    wave_header hdr;

    std::ifstream i { path, std::ios::binary };
    HAL_ASSERT(i.is_open(), "Couldn't open ", path);

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

    std::unique_ptr<char[]> alloc { std::make_unique<char[]>(hdr.data_size) };
    i.read(alloc.get(), hdr.data_size);

    return { alloc.get(), std::size_t(hdr.data_size), get_format(hdr.channels, hdr.bits_per_sample), hdr.sample_rate, {} };
}

buffer decoder::mp3(const std::filesystem::path& path)
{
    mp3dec_t           mp3d;
    mp3dec_file_info_t info;

    HAL_ASSERT_VITAL(::mp3dec_load(&mp3d, path.c_str(), &info, nullptr, nullptr) == 0, "Could not decode ", path);

    buffer b { info.buffer, info.samples * sizeof(mp3d_sample_t), stereo16, info.hz, {} };

    std::free(info.buffer);

    return b;
}