#pragma once

#include <SDL_rwops.h>

#include <halcyon/internal/raii_object.hpp>

#include <halcyon/utility/concepts.hpp>
#include <halcyon/utility/pass_key.hpp>

namespace hal
{
    class surface;

    namespace image
    {
        class context;
    }

    namespace ttf
    {
        class context;
    }

    namespace detail
    {
        // Base class for SDL_RWops operations.
        class rwops : public detail::raii_object<SDL_RWops, ::SDL_RWclose>
        {
        protected:
            using raii_object::raii_object;

            template <typename T>
            static const char* string_data(const T& obj)
            {
                if constexpr (std::is_same_v<meta::remove_pointer_to_const<T>, char*>)
                    return obj;

                else
                    return std::data(obj);
            }
        };
    }

    namespace meta
    {
        // A type that can be used as a string.
        template <typename T>
        concept rwops_path = std::is_assignable_v<std::string_view, T>;

        // A type that represents a static/dynamic array.
        // The element type must be 1 byte large.
        template <typename T>
        concept rwops_buffer = !rwops_path<T> && buffer<T>;
    }

    // An abstraction of various methods of accessing data.
    class accessor : public detail::rwops
    {
    public:
        // Access a file.
        // This constructor accepts anything that can be non-explicitly assigned to a std::string_view.
        // Consult the meta::string_like concept for more info.
        template <meta::rwops_path T>
        accessor(const T& path)
            : rwops { ::SDL_RWFromFile(string_data(path), "r") }
        {
        }

        // Access a buffer.
        // This constructor accepts any object that cannot be non-explicitly assigned to a std::string_view.
        // In addition, std::data() and std::size() must be available for use with it.
        // Consult the meta::accessor_buffer concept for more info.
        template <meta::rwops_buffer T>
        accessor(const T& buffer)
            requires(sizeof(std::remove_pointer_t<decltype(std::data(buffer))>) == 1)
            : rwops { ::SDL_RWFromConstMem(std::data(buffer), std::size(buffer)) }
        {
        }

        // get() functions seek the RWops back where they started.
        SDL_RWops* get(pass_key<image::context>) const; // Image format querying.

        // use() functions call release(), so the class gets "consumed".
        SDL_RWops* use(pass_key<surface>);        // BMP loading.
        SDL_RWops* use(pass_key<image::context>); // Image loading.
        SDL_RWops* use(pass_key<ttf::context>);   // Font loading.
    };

    // An abstraction of various methods of outputting data.
    class outputter : public detail::rwops
    {
    public:
        // Output to a file.
        // This accepts anything that can be assigned (not explicitly) to a std::string_view.
        // Consult the meta::string_like concept for more info.
        template <meta::rwops_path T>
        outputter(const T& path)
            : rwops { ::SDL_RWFromFile(string_data(path), "w") }
        {
        }

        // Output data to an array.
        // This constructor accepts any container that cannot be assigned to a std::string_view.
        // In addition, std::data() and std::size() must be available for use with it.
        // Consult the meta::outputter_buffer concept for more info.
        template <meta::rwops_buffer T>
        outputter(T& buffer)
            : rwops { ::SDL_RWFromMem(std::data(buffer), std::size(buffer)) }
        {
        }

        // use() functions call release(), so the class gets "consumed".
        SDL_RWops* use(pass_key<surface>);        // BMP saving.
        SDL_RWops* use(pass_key<image::context>); // Image saving.
    };
}