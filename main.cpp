#include <halcyon/events/animation.hpp>
#include <halcyon/mono_app.hpp>

int main(int argc, char* argv[]) {

    hal::mono_app game{"Interloper 1.1"};

    game.mixer.mus.play("../assets/ost/The Way Home.mp3", hal::infinite_loop);

    const hal::font m5x7{game.ttf.load_font("../assets/fonts/m5x7.ttf", 72)};
    const hal::surface surf{game.window, game.window.size()};

    ::SDL_BlitScaled(game.image.load("test.jpg").ptr(), nullptr, surf.ptr(),
                     nullptr);

    while (game.update() && !game.input().pressed(hal::button::esc)) {

        if (game.input().held(hal::button::backspace))
            HAL_CONSOLE_LOG(hal::severity::warning, "Backspace held");

        if (game.input().held(hal::button::lmb))
            HAL_CONSOLE_LOG(hal::severity::warning, "LMB held");

        game.window.renderer.set_fill(surf[game.input().mouse()]);

        HAL_CONSOLE_DRAW(m5x7, game.window);
    }

    return EXIT_SUCCESS;
}