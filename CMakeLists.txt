project(hello)

include_directories( C:\Users\tmp\personal\dev\cpp\sdl_cmake\vendor\SDL2\SDL2_win\include )

link_directories( C:\Users\tmp\personal\dev\cpp\sdl_cmake\vendor\SDL2\SDL2_win\lib\x64 )

add_executable( hello main.cpp )
target_link_libraries( hello SDL2 SDL2main )
