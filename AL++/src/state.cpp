#include <AL++/debug.hpp>
#include <AL++/state.hpp>

using namespace alpp;

std::string_view state::query(query_enum what)
{
    return ALPP_AL_CALL(::alGetString, what);
}