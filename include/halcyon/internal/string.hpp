#pragma once

#include <halcyon/internal/raii_object.hpp>
#include <halcyon/utility/pass_key.hpp>

namespace hal
{
    namespace proxy
    {
        class clipboard;
    }

    // SDL sometimes returns string pointers that we have to free via
    // its own function afterwards. This is a wrapper of that functionality
    // that attempts to mimic std::string as best as it can.
    class string : public detail::raii_object<char, ::SDL_free>
    {
    public:
        using authority_t = proxy::clipboard;

        string(char* ptr, pass_key<authority_t>);

        std::size_t size() const;

        char* begin();
        char* end();

        const char* begin() const;
        const char* end() const;

        char*       data();
        const char* data() const;

        const char* c_str() const;

        operator std::string_view() const;
    };

    bool operator==(string lhs, std::string_view rhs);
}