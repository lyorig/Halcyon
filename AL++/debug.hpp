#pragma once

#include <halcyon/debug.hpp>

#ifdef HAL_DEBUG_ENABLED

    #include <AL++/types.hpp>

namespace alpp
{
    template <typename T>
    std::ostream& operator<<(std::ostream& str, const point3<T>& pt)
    {
        return str << '[' << hal::to_printable_int(pt.x) << ' ' << hal::to_printable_int(pt.y) << ' ' << hal::to_printable_int(pt.z) << ']';
    }

    namespace al
    {
        enum error
        {
            no_error          = AL_NO_ERROR,
            invalid_name      = AL_INVALID_NAME,
            invalid_enum      = AL_INVALID_ENUM,
            invalid_value     = AL_INVALID_VALUE,
            invalid_operation = AL_INVALID_OPERATION,
            out_of_memory     = AL_OUT_OF_MEMORY
        };

        std::string error_string(enum_t err);
        void        check_errors(std::string_view func, std::string_view al_func);

        template <typename Ret, typename... FuncArgs, typename... GivenArgs>
        Ret call(std::string_view func_name, std::string_view al_func_name, lyo::func_ptr<Ret, FuncArgs...> func, GivenArgs&&... args)
            requires(sizeof...(FuncArgs) == sizeof...(GivenArgs) && (lyo::static_castable<GivenArgs, FuncArgs> && ...))
        {
            ::alGetError(); // Reset error state.

            if constexpr (std::is_void_v<Ret>)
            {
                func(static_cast<FuncArgs>(args)...);
                check_errors(func_name, al_func_name);
            }

            else
            {
                const Ret rv { func(static_cast<FuncArgs>(args)...) };
                check_errors(func_name, al_func_name);
                return rv;
            }
        }
    }

    namespace alc
    {
        enum error
        {
            no_error        = ALC_NO_ERROR,
            invalid_device  = ALC_INVALID_DEVICE,
            invalid_context = ALC_INVALID_CONTEXT,
            invalid_enum    = ALC_INVALID_ENUM,
            invalid_value   = ALC_INVALID_VALUE,
            out_of_memory   = ALC_OUT_OF_MEMORY
        };

        std::string error_string(enum_t err);
        void        check_errors(std::string_view func, std::string_view al_func, ALCdevice* dev);

        // Boolean-returning ALC functions get processed inside this function.
        template <typename T>
        using call_ret_type = std::conditional_t<lyo::is_present_v<T, void, bool_t>, void, T>;

        template <typename Ret, typename... FuncArgs, typename... GivenArgs>
        call_ret_type<Ret> call(std::string_view func_name, std::string_view al_func_name, ALCdevice* dev, lyo::func_ptr<Ret, FuncArgs...> func, GivenArgs&&... args)
            requires(sizeof...(FuncArgs) == sizeof...(GivenArgs) && (lyo::static_castable<GivenArgs, FuncArgs> && ...))
        {
            ::alcGetError(dev); // Reset error state.

            if constexpr (std::is_void_v<Ret>)
            {
                func(static_cast<FuncArgs>(args)...);
                check_errors(func_name, al_func_name, dev);
            }

            else if constexpr (std::is_same_v<Ret, bool_t>)
            {
                if (!func(static_cast<FuncArgs>(args)...))
                    HAL_PANIC(func_name, " failed");
            }

            else
            {
                return func(static_cast<FuncArgs>(args)...);
            }
        }
    }
}

    #define ALPP_AL_CALL(func, ...)       alpp::al::call(__PRETTY_FUNCTION__, #func, func, __VA_ARGS__)
    #define ALPP_ALC_CALL(dev, func, ...) alpp::alc::call(__PRETTY_FUNCTION__, #func, dev, func, __VA_ARGS__)

#else

    #define ALPP_AL_CALL(func, ...)       func(__VA_ARGS__)
    #define ALPP_ALC_CALL(dev, func, ...) func(__VA_ARGS__)

#endif
