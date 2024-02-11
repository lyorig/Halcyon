project('quest', 'cpp', default_options: ['cpp_std=c++23'])

halcyon_deps = [
    dependency('sdl2'),
    dependency('sdl2_image'),
    dependency('sdl2_ttf'),
    dependency('OpenAL')
]

halcyon_inc = [
    include_directories('halcyon'),
    include_directories('lyo')
]

quest_inc = include_directories('quest')

halcyon_rawsrc = [
    'debug.cpp', 'engine.cpp', 'image_loader.cpp', 'input_handler.cpp', 'renderer.cpp', 'texture.cpp', 'ttf_engine.cpp', 'window.cpp',
    'components/font.cpp', 'components/surface.cpp', 'audio/base.cpp', 'audio/listener.cpp','audio/source.cpp', 'audio/buffer.cpp', 'audio/decoder.cpp', 'internal/printing.cpp', 'internal/subsystem.cpp'
]

quest_rawsrc = [
    'game.cpp',
    'states/base.cpp', 'states/intro.cpp', 'states/menu.cpp', 'states/paused.cpp', 'states/playing.cpp', 
]

halcyon_src = []
quest_src = []

foreach file : halcyon_rawsrc
    full_path = join_paths('halcyon', 'src', file)
    halcyon_src += files(full_path)
endforeach

foreach file : quest_rawsrc
    full_path = join_paths('quest', 'src', file)
    quest_src += files(full_path)
endforeach

executable('HalodaQuest', win_subsystem: 'windows', dependencies: halcyon_deps,
include_directories: [halcyon_inc, quest_inc], sources: ['quest/main.cpp', halcyon_src, quest_src])

executable('HalChess', win_subsystem: 'windows', dependencies: halcyon_deps,
include_directories: [halcyon_inc], sources: ['chess/main.cpp', halcyon_src, quest_src])

executable('HalTest', win_subsystem: 'windows', dependencies: halcyon_deps, include_directories: halcyon_inc, sources: ['test/main.cpp', halcyon_src])
