#include "halcyon.hpp"
#include "halcyon/components/renderer.hpp"
#include "halcyon/window.hpp"

using namespace hal;

cyon::cyon(const char *window_title) noexcept
    : m_input{m_eng}, mixer{m_eng}, window{m_eng, window_title, hal::fullscreen, {renderer::accelerated, renderer::vsync}},
      image{window, {image.jpg, image.png}}, ttf{window} {}

bool cyon::update() noexcept {
  window.present();

  return m_input.update();
}

const input_handler &cyon::input() const noexcept {
  return m_input;
}