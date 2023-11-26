#include "halcyon/types/render.hpp"
#include <halcyon/events/animation.hpp>
#include <halcyon/mono_app.hpp>

int main(int argc, char* argv[]) {

    hal::mono_app game{"Interloper 1.1"};

    game.mixer.mus.play("assets/ost/The Way Home.mp3", hal::infinite_loop);

    const hal::font m5x7{game.ttf.load_font("assets/fonts/m5x7.ttf", 72)};

    const hal::texture tex{
        game.window,
        game.image.load("test.jpg").resize(game.window.size() / 2)};

    while (game.update() && !game.input().pressed(hal::button::esc)) {

        if (game.input().held(hal::button::backspace))
            HAL_CONSOLE_LOG(hal::severity::warning, "Backspace held");

        if (game.input().held(hal::button::lmb))
            HAL_CONSOLE_LOG(hal::severity::warning, "LMB held");

        tex.draw(hal::anchor::center);

        HAL_CONSOLE_DRAW(m5x7, game.window);
    }

    return EXIT_SUCCESS;
}