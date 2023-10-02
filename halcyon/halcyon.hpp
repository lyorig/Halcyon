#pragma once

#include "lyo/types.hpp"
#include "engine.hpp"
#include "mixer.hpp"
#include "window.hpp"
#include "input_handler.hpp"
#include "image_loader.hpp"
#include "ttf_engine.hpp"

namespace hal {
class cyon {

  MAYBE_EMPTY engine m_eng;
  input_handler m_input;

public:
  mixer mixer;

  window window;

  MAYBE_EMPTY image_loader image;
  MAYBE_EMPTY ttf_engine ttf;

  // TODO: Add more constructor options in a... humane way.
  cyon(const char *window_title) noexcept;

  bool update() noexcept;

  const input_handler& input() const noexcept;
};
}