#pragma once

#include <cstring>
#include <optional>
#include <sstream>

/* argparse.hpp:
   Argument-parsing functions. */

namespace lyo {
class parser {
public:
    parser(const int& argc, const char* const* const& argv) noexcept
        : m_argv { argv }
        , m_argc { argc }
    {
    }

    bool has(const char* name) const noexcept
    {
        for (int i { 0 }; i < m_argc; ++i) {
            if (std::strcmp(m_argv[i], name) == 0)
                return true;
        }

        return false;
    }

    template <typename T>
    std::optional<T> parse(const char* prefix) const noexcept
    {
        const char* tok { nullptr };

        for (int i { 0 }; i < m_argc; ++i) {
            if ((tok = strstr(m_argv[i], prefix)) != nullptr)
                break;
        }

        if (!tok) // Nothing found, GTFO.
            return std::nullopt;

        T ret;

        std::istringstream s { tok + std::strlen(prefix) };

        return (s >> ret) ? ret : std::nullopt;
    }

    template <typename T>
    T parse(const char* prefix, T default_value) const noexcept
    {
        const char* tok { nullptr };

        for (int i { 0 }; i < m_argc; ++i) {
            if ((tok = strstr(m_argv[i], prefix)) != nullptr)
                break;
        }

        if (!tok) // Nothing found, GTFO.
            return default_value;

        T ret;

        std::istringstream s { tok + std::strlen(prefix) };

        return (s >> ret) ? ret : default_value;
    }

private:
    const char* const* const& m_argv; // "How many consts would you like?" "Yes."
    const int& m_argc;
};
}