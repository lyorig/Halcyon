#include <AL++/debug.hpp>
#include <AL++/state.hpp>
#include <vector>

using namespace alpp;

std::string_view state::query(query_type what)
{
    return ALPP_AL_CALL(::alGetString, what);
}

std::string state::query(null_alc_query_type what)
{
    const char* str { ::alcGetString(nullptr, what) };
    std::string ret;

    do
    {
        const auto sz = std::strlen(str);
        ret += str;
        ret += '/';
        str += sz;
    } while (*(++str) != '\0');

    ret.pop_back();

    return ret;
}