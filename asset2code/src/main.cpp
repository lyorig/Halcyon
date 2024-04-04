#include <filesystem>
#include <iostream>
#include <span>

#include <halcyon/cleanup.hpp>
#include <halcyon/other/clipboard.hpp>

constexpr std::size_t on_line { 10 };

std::size_t approx_size(std::size_t file_size)
{
    // [0xFF,] + outer braces + linebreak amount
    return file_size * 5 + 2 + file_size / on_line;
}

void put(std::string& str, std::byte val)
{
    char buffer[6];
    (void)std::snprintf(buffer, sizeof(buffer), "0x%x,", static_cast<std::uint8_t>(val));
    str += buffer;
}

int main(int argc, char* argv[])
{
    if (argc == 1)
    {
        std::puts("No file specified.");
        return EXIT_FAILURE;
    }

    // We have a file argument.
    std::FILE* f { std::fopen(argv[1], "r") };
    if (f == nullptr)
    {
        std::cout << "Could not open \"" << argv[1] << "\".\n";
        return EXIT_FAILURE;
    }

    const auto size = std::filesystem::file_size(argv[1]);

    std::unique_ptr<std::byte[]> bytes { std::make_unique<std::byte[]>(size) };
    std::fread(bytes.get(), 1, size, f);

    std::string res { '{' };
    res.reserve(approx_size(size));

    std::span<const std::byte> sp { bytes.get(), size };
    std::size_t                line_amnt { 0 };

    for (auto b : sp)
    {
        put(res, b);
        if (++line_amnt == on_line)
        {
            line_amnt = 0;
            res.push_back('\n');
        }
    }

    res += '}';

    hal::cleanup c { hal::system::video };

    hal::clipboard::set(res);
    std::cout << "Done, copied " << res.size() << " bytes.\n";
    return EXIT_SUCCESS;
}