#include <halcyon/events/animation.hpp>
#include <halcyon/halcyon.hpp>

int main(int argc, char *argv[]) {
  hal::cyon game{"Interloper v1.1"};

  game.mixer.mus.play("assets/ost/The Way Home.mp3", hal::infinite_loop);

  const hal::font m5x7{game.ttf.load_font("assets/fonts/m5x7.ttf", 72)};
  const hal::texture tex{game.window,
                         m5x7.render("Made with Halcyon", hal::color::black)};

  hal::color color{hal::color::white};

  hal::animation anim{color, [&](hal::color &c, lyo::f64 t) {
                        using vt = hal::color::value_type;

                        c.r = static_cast<vt>(std::abs(std::sin(t)) *
                                              lyo::f64(0x80));

                        game.window.renderer.set_fill(color);
                      }};

  const auto size{tex.vw(50.0)};

  while (game.update() && !game.input().pressed(hal::button::esc)) {
    tex.draw(hal::anchor::center, size);

    anim.update();
  }

  return EXIT_SUCCESS;
}