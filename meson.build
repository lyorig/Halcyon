project('quest', 'cpp', default_options: ['cpp_std=c++23'])

sdl2 = dependency('sdl2')
sdl2image = dependency('sdl2_image')
sdl2ttf = dependency('sdl2_ttf')
sdl2mixer = dependency('sdl2_mixer')

halcyon = include_directories('halcyon')
lyo = include_directories('lyo')
quest = include_directories('quest')

halcyon_rawsrc = [
    'debug.cpp', 'engine.cpp', 'image_loader.cpp', 'input_handler.cpp', 'mixer.cpp', 'renderer.cpp', 'texture.cpp', 'ttf_engine.cpp', 'window.cpp',
    'components/chunk.cpp', 'components/font.cpp', 'components/music.cpp', 'components/surface.cpp', 'internal/printing.cpp', 'internal/subsystem.cpp'
]

halcyon_src = []

foreach file : halcyon_rawsrc
    full_path = join_paths('halcyon', 'src', file)
    halcyon_src += files(full_path)
endforeach

quest_rawsrc = [
    'game.cpp', 'main.cpp',
    'states/base.cpp', 'states/intro.cpp', 'states/menu.cpp', 'states/paused.cpp', 'states/playing.cpp', 
]

quest_src = []

foreach file : quest_rawsrc
    full_path = join_paths('quest', 'src', file)
    quest_src += files(full_path)
endforeach

executable('HalodaQuest', win_subsystem: 'windows', dependencies: [sdl2, sdl2image, sdl2ttf, sdl2mixer],
include_directories: [lyo, halcyon, quest], sources: [halcyon_src, quest_src])